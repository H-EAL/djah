#include "djah/3d/primitives/sphere.hpp"
#include "djah/math/vector3.hpp"
#include "djah/geometry/triangle.hpp"

namespace djah { namespace d3d { namespace primitives {

	//----------------------------------------------------------------------------------------------
	std::vector<triangle> iso_sphere::construct(int level, int)
	{
		std::vector< std::vector<triangle> > triangles(level+1);

		for(int i = 0; i <= level; ++i)
		{
			const unsigned int nbTriangles = 1 << (3 + (i * 2));
			triangles[i].resize( nbTriangles  );
		}

		const math::vector3f bottom( 0,-1, 0);
		const math::vector3f top   ( 0, 1, 0);
		const math::vector3f left  (-1, 0, 0);
		const math::vector3f right ( 1, 0, 0);
		const math::vector3f front ( 0, 0, 1);
		const math::vector3f back  ( 0, 0,-1);

		// Top
		setTriangle(triangles[0][0], top, left , front);
		setTriangle(triangles[0][1], top, front, right);
		setTriangle(triangles[0][2], top, right, back );
		setTriangle(triangles[0][3], top, back , left );
		// Bottom
		setTriangle(triangles[0][4], left , bottom, front);
		setTriangle(triangles[0][5], front, bottom, right);
		setTriangle(triangles[0][6], right, bottom, back );
		setTriangle(triangles[0][7], back , bottom, left );

		for(int i = 1; i <= level; ++i)
		{
			int ind = 0;
			auto itEnd = triangles[i-1].end();
			for(auto it = triangles[i-1].begin(); it != itEnd; ++it)
			{
				const triangle &t = (*it);

				const math::vector3f &ab = ((t.pointA() + t.pointB()) / 2.0f).normalize();
				const math::vector3f &bc = ((t.pointB() + t.pointC()) / 2.0f).normalize();
				const math::vector3f &ca = ((t.pointC() + t.pointA()) / 2.0f).normalize();

				setTriangle(triangles[i][ind++], t.pointA(), ab, ca);
				setTriangle(triangles[i][ind++], ab, t.pointB(), bc);
				setTriangle(triangles[i][ind++], bc, t.pointC(), ca);
				setTriangle(triangles[i][ind++], ab, bc, ca);
			}
		}

		return triangles[level];
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void iso_sphere::setTriangle
	(
		triangle &tri,
		const math::vector3f &a,
		const math::vector3f &b,
		const math::vector3f &c
	)
	{
		geometry::triangle3f t(a,b,c);

		tri.setPoints(a, b, c);
		tri.setNormals(a, b, c);
		tri.setTextureCoordinates
		(
			computeTextureCoordinates(a, t.normal()),
			computeTextureCoordinates(b, t.normal()),
			computeTextureCoordinates(c, t.normal())
		);

		if( std::abs(tri.normalA().y) == 1.0f )
		{
			tri.textureCoordinatesA().x = (tri.textureCoordinatesB().x + tri.textureCoordinatesC().x) / 2.0f;
		}
		else if( std::abs(tri.normalB().y) == 1.0f )
		{
			tri.textureCoordinatesB().x = (tri.textureCoordinatesA().x + tri.textureCoordinatesC().x) / 2.0f;
		}
		else if( std::abs(tri.normalC().y) == 1.0f )
		{
			tri.textureCoordinatesC().x = (tri.textureCoordinatesA().x + tri.textureCoordinatesB().x) / 2.0f;
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	math::vector2f iso_sphere::computeTextureCoordinates
	(
		const math::vector3f &n,
		const math::vector3f &tn
	)
	{
		math::vector2f tc;
		tc.x = (atan2(-n.z, n.x) / math::pi + 1.0f) * 0.5f;
		tc.y = acos( -n.y ) / math::pi;

		if( tn.x > 0.0f && tn.z > 0.0f && tc.x < 0.75f )
		{
			tc.x = 1.0f - tc.x;
		}
		return tc;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	std::vector<triangle> sphere::construct(int slices, int stacks)
	{
		slices = std::max(3, slices);
		stacks = std::max(2, stacks);

		std::vector<triangle> triangles(slices * (2*(stacks-2) + 2));

		int ind = 0;
		const float longStep = math::pi_times_2 / (float)slices;
		const float latStep = math::pi / (float)stacks;

		const math::point3f top(0.0f, 1.0f, 0.0f);
		const math::point3f bottom(0.0f, -1.0f, 0.0f);

		for(float longCursor = 0.0f; longCursor < math::pi_times_2; longCursor += longStep)
		{
			const float cosLong = cos(longCursor);
			const float sinLong = sin(longCursor);
			const float cosNextLong = cos(longCursor+longStep);
			const float sinNextLong = sin(longCursor+longStep);

			const float u = 1.0f - longCursor / math::pi_times_2;
			const float nextU = 1.0f - (longCursor+longStep) / math::pi_times_2;

			for(float latCursor = 0.0f; latCursor < math::pi; latCursor += latStep)
			{
				const float cosLat = cos(latCursor);
				const float sinLat = sin(latCursor);
				const float cosNextLat = cos(latCursor+latStep);
				const float sinNextLat = sin(latCursor+latStep);

				math::point3f longLat
				(
					sinLat * cosLong,
					cosLat,
					sinLat * sinLong
				);

				math::point3f longNextLat
				(
					sinNextLat * cosLong,
					cosNextLat,
					sinNextLat * sinLong
				);

				math::point3f nextLongLat
				(
					sinLat * cosNextLong,
					cosLat,
					sinLat * sinNextLong
				);

				math::point3f nextLongNextLat
				(
					sinNextLat * cosNextLong,
					cosNextLat,
					sinNextLat * sinNextLong
				);

				const float v = 1.0f - latCursor / math::pi;
				const float nextV = 1.0f - (latCursor+latStep) / math::pi;

				if( latCursor > 0.0f )
				{
					triangles[ind].setPoints(longLat, longNextLat, nextLongLat);
					triangles[ind].setNormals(longLat, longNextLat, nextLongLat);
					triangles[ind++].setTextureCoordinates(math::point2f(u,v), math::point2f(u,nextV), math::point2f(nextU,v));
				}
				if( latCursor < math::pi-latStep*1.1f )
				{
					triangles[ind].setPoints(nextLongLat, longNextLat, nextLongNextLat);
					triangles[ind].setNormals(nextLongLat, longNextLat, nextLongNextLat);
					triangles[ind++].setTextureCoordinates(math::point2f(nextU,v), math::point2f(u,nextV), math::point2f(nextU,nextV));
				}
			}
		}

		return triangles;
	}
	//----------------------------------------------------------------------------------------------

} /*primitives*/ } /*d3d*/ } /*djah*/