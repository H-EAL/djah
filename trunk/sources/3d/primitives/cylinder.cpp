#include "djah/3d/primitives/cylinder.hpp"
#include "djah/math/vector3.hpp"
#include "djah/geometry/triangle.hpp"

namespace djah { namespace d3d { namespace primitives {

	//----------------------------------------------------------------------------------------------
	std::vector<triangle> cylinder::construct(int level)
	{
		level = std::max(3, level);
		std::vector<triangle> triangles;
		triangles.resize(level * 4);

		const float h = 0.5f;

		const math::point3f cirleTop( 0.0f,h,0.0f );
		const math::point3f circleBottom( 0.0f,-h,0.0f );

		int ind = 0;
		const float step = math::pi_times_2 / (float)level;
		for(float cursor = 0.0f; cursor < math::pi_times_2; cursor += step)
		{
			const math::point3f topPt( cos(cursor), h, sin(cursor) );
			const math::point3f bottomPt( topPt.x, -h, topPt.z );
			const math::vector3f sideN( topPt.x, 0.0f, topPt.z );

			const math::point3f nextTopPt( cos(cursor+step), h, sin(cursor+step) );
			const math::point3f nextBottomPt( nextTopPt.x, -h, nextTopPt.z );
			const math::vector3f nextSideN( nextTopPt.x, 0.0f, nextTopPt.z );

			triangles[ind++] = geometry::triangle3f(cirleTop, topPt, nextTopPt);

			triangles[ind] = geometry::triangle3f(topPt, bottomPt, nextTopPt);
			triangles[ind++].setNormals(sideN, sideN, nextSideN);

			triangles[ind] = geometry::triangle3f(nextTopPt, bottomPt, nextBottomPt);
			triangles[ind++].setNormals(nextSideN, sideN, nextSideN);

			triangles[ind++] = geometry::triangle3f(bottomPt, circleBottom, nextBottomPt);
		}

		return triangles;
	}
	//----------------------------------------------------------------------------------------------

} /*primitives*/ } /*d3d*/ } /*djah*/