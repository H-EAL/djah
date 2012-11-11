#ifndef DJAH_VIDEO_PRIMITIVES_CUBE_HPP
#define DJAH_VIDEO_PRIMITIVES_CUBE_HPP

#include <vector>
#include "../../geometry/triangle.hpp"
#include "../../math/vector3.hpp"

namespace djah { namespace video { namespace primitives {

	class cube
	{
	public:
		/*
		const float cubeVertices[] = 
		{
			//Position - Color//
			0,0,0,     0,0,0,    // 0
			1,0,0,     1,0,0,    // 1
			1,1,0,     1,1,0,    // 2
			0,1,0,     0,1,0,    // 3

			0,0,1,     0,0,1,    // 4
			1,0,1,     1,0,1,    // 5 
			1,1,1,     1,1,1,    // 6
			0,1,1,     0,1,1,    // 7
		};

		const u8 cubeIndices[] =
		{
			0,3,2, 0,2,1,
			1,2,6, 1,6,5,
			5,6,7, 5,7,4,
			4,7,3, 4,3,0,
			3,7,6, 3,6,2,
			1,5,4, 1,4,0,
		};
		*/
		template<int>
		std::vector<geometry::triangle3f> construct()
		{
			std::vector<geometry::triangle3f> triangles;
			triangles.reserve(12);

			const float size = 0.5f;

			const math::vector3f leftTopFront    (-size, size, size);
			const math::vector3f rightTopFront   ( size, size, size);
			const math::vector3f leftBottomFront (-size,-size, size);
			const math::vector3f rightBottomFront( size,-size, size);

			const math::vector3f leftTopBack     (-size, size,-size);
			const math::vector3f rightTopBack    ( size, size,-size);
			const math::vector3f leftBottomBack  (-size,-size,-size);
			const math::vector3f rightBottomBack ( size,-size,-size);

			// Front
			triangles.push_back( geometry::triangle3f(leftBottomFront, rightBottomFront, rightTopFront) );
			triangles.push_back( geometry::triangle3f(leftBottomFront, rightTopFront, leftTopFront) );
			// Right
			triangles.push_back( geometry::triangle3f(rightBottomFront, rightBottomBack, rightTopBack) );
			triangles.push_back( geometry::triangle3f(rightBottomFront, rightTopBack, rightTopFront) );
			// Back
			triangles.push_back( geometry::triangle3f(rightBottomBack, leftBottomBack, leftTopBack) );
			triangles.push_back( geometry::triangle3f(rightBottomBack, leftTopBack, rightTopBack) );
			// Left
			triangles.push_back( geometry::triangle3f(leftBottomBack, leftBottomFront, leftTopFront) );
			triangles.push_back( geometry::triangle3f(leftBottomBack, leftTopFront, leftTopBack) );
			// Top
			triangles.push_back( geometry::triangle3f(leftTopFront, rightTopFront, rightTopBack) );
			triangles.push_back( geometry::triangle3f(leftTopFront, rightTopBack, leftTopBack) );
			// Bottom
			triangles.push_back( geometry::triangle3f(leftBottomBack, rightBottomBack, rightBottomFront) );
			triangles.push_back( geometry::triangle3f(leftBottomBack, rightBottomFront, leftBottomFront) );

			return triangles;
		}
	};

} /*primitives*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_PRIMITIVES_CUBE_HPP */
