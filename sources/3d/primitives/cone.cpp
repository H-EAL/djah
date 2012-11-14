#include "djah/3d/primitives/cone.hpp"
#include "djah/math/vector3.hpp"
#include "djah/geometry/triangle.hpp"

namespace djah { namespace d3d { namespace primitives {

	//----------------------------------------------------------------------------------------------
	std::vector<triangle> cone::construct(int level)
	{
		level = std::max(3, level);
		std::vector<triangle> triangles;
		triangles.resize(level * 2);

		const float h = 1.0f;

		const math::point3f top( 0.0f,1.0f,0.0f );
		const math::point3f circleBottom( 0.0f,0.0f,0.0f );

		int ind = 0;
		const float step = math::pi_times_2 / (float)level;
		for(float cursor = 0.0f; cursor < math::pi_times_2; cursor += step)
		{
			const math::point3f pt( cos(cursor), 0.0f, sin(cursor) );
			const math::point3f nextPt( cos(cursor+step), 0.0f, sin(cursor+step) );

			triangles[ind] = geometry::triangle3f(top, pt, nextPt);
			triangles[ind++].setNormals(top, pt, nextPt);

			triangles[ind++] = geometry::triangle3f(pt, circleBottom, nextPt);
		}

		return triangles;
	}
	//----------------------------------------------------------------------------------------------

} /*primitives*/ } /*d3d*/ } /*djah*/