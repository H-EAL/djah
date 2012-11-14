#include "djah/3d/primitives/sphere.hpp"
#include "djah/math/vector3.hpp"
#include "djah/geometry/triangle.hpp"

namespace djah { namespace d3d { namespace primitives {

	//----------------------------------------------------------------------------------------------
	std::vector<triangle> iso_sphere::construct(int level)
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

} /*primitives*/ } /*d3d*/ } /*djah*/