#include "djah/3d/primitives/ribbon.hpp"
#include "djah/math/vector3.hpp"
#include "djah/geometry/triangle.hpp"

namespace djah { namespace d3d { namespace primitives {

	//----------------------------------------------------------------------------------------------
	std::vector<triangle> ribbon::construct(int slices, int)
	{
		slices = std::max(3, slices);
		std::vector<triangle> triangles(slices * 2);

		const float h = 0.5f;

		int ind = 0;
		const float longStep = math::pi_times_2 / (float)slices;
		for(float cursor = 0.0f; cursor < math::pi_times_2; cursor += longStep)
		{
			const math::point3f topPt( cos(cursor), h, sin(cursor) );
			const math::point3f bottomPt( topPt.x, -h, topPt.z );
			const math::vector3f sideN( topPt.x, 0.0f, topPt.z );

			const math::point3f nextTopPt( cos(cursor+longStep), h, sin(cursor+longStep) );
			const math::point3f nextBottomPt( nextTopPt.x, -h, nextTopPt.z );
			const math::vector3f nextSideN( nextTopPt.x, 0.0f, nextTopPt.z );

			const float u = 1.0f - cursor / math::pi_times_2;
			const float nextU = 1.0f - (cursor+longStep) / math::pi_times_2;

			triangles[ind].setPoints(topPt, bottomPt, nextTopPt);
			triangles[ind].setNormals(sideN, sideN, nextSideN);
			triangles[ind++].setTextureCoordinates(math::point2f(u,1.0f), math::point2f(u,0.0f), math::point2f(nextU,1.0f));

			triangles[ind].setPoints(nextTopPt, bottomPt, nextBottomPt);
			triangles[ind].setNormals(nextSideN, sideN, nextSideN);
			triangles[ind++].setTextureCoordinates(math::point2f(nextU,1.0f), math::point2f(u,0.0f), math::point2f(nextU,0.0f));
		}

		return triangles;
	}
	//----------------------------------------------------------------------------------------------

} /*primitives*/ } /*d3d*/ } /*djah*/