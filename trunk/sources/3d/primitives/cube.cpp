#include "djah/3d/primitives/cube.hpp"
#include "djah/math/vector3.hpp"
#include "djah/geometry/triangle.hpp"

namespace djah { namespace d3d { namespace primitives {

	/***************************************
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
	****************************************/

	//----------------------------------------------------------------------------------------------
	std::vector<triangle> cube::construct(int,int)
	{
		std::vector<triangle> triangles(12);

		const float size = 0.5f;

		const math::point3f leftTopFront    (-size, size, size);
		const math::point3f rightTopFront   ( size, size, size);
		const math::point3f leftBottomFront (-size,-size, size);
		const math::point3f rightBottomFront( size,-size, size);

		const math::point3f leftTopBack     (-size, size,-size);
		const math::point3f rightTopBack    ( size, size,-size);
		const math::point3f leftBottomBack  (-size,-size,-size);
		const math::point3f rightBottomBack ( size,-size,-size);

		// Texture coordinates
		const math::point2f tcLB(0.0f,0.0f);
		const math::point2f tcLT(0.0f,1.0f);
		const math::point2f tcRT(1.0f,1.0f);
		const math::point2f tcRB(1.0f,0.0f);

		int ind = 0;

		// Front
		triangles[ind] = triangle(geometry::triangle3f(leftBottomFront, rightTopFront, rightBottomFront));
		triangles[ind++].setTextureCoordinates(tcLB, tcRT, tcRB);
		triangles[ind] = triangle(geometry::triangle3f(leftBottomFront, leftTopFront, rightTopFront));
		triangles[ind++].setTextureCoordinates(tcLB, tcLT, tcRT);
		// Right
		triangles[ind] = triangle(geometry::triangle3f(rightBottomFront, rightTopBack, rightBottomBack));
		triangles[ind++].setTextureCoordinates(tcLB, tcRT, tcRB);
		triangles[ind] = triangle(geometry::triangle3f(rightBottomFront, rightTopFront, rightTopBack));
		triangles[ind++].setTextureCoordinates(tcLB, tcLT, tcRT);
		// Back
		triangles[ind] = triangle(geometry::triangle3f(rightBottomBack, leftTopBack, leftBottomBack));
		triangles[ind++].setTextureCoordinates(tcLB, tcRT, tcRB);
		triangles[ind] = triangle(geometry::triangle3f(rightBottomBack, rightTopBack, leftTopBack));
		triangles[ind++].setTextureCoordinates(tcLB, tcLT, tcRT);
		// Left
		triangles[ind] = triangle(geometry::triangle3f(leftBottomBack, leftTopFront, leftBottomFront));
		triangles[ind++].setTextureCoordinates(tcLB, tcRT, tcRB);
		triangles[ind] = triangle(geometry::triangle3f(leftBottomBack, leftTopBack, leftTopFront));
		triangles[ind++].setTextureCoordinates(tcLB, tcLT, tcRT);
		// Top
		triangles[ind] = triangle(geometry::triangle3f(leftTopFront, rightTopBack, rightTopFront));
		triangles[ind++].setTextureCoordinates(tcLB, tcRT, tcRB);
		triangles[ind] = triangle(geometry::triangle3f(leftTopFront, leftTopBack, rightTopBack));
		triangles[ind++].setTextureCoordinates(tcLB, tcLT, tcRT);
		// Bottom
		triangles[ind] = triangle(geometry::triangle3f(leftBottomBack, rightBottomFront, rightBottomBack));
		triangles[ind++].setTextureCoordinates(tcLB, tcRT, tcRB);
		triangles[ind] = triangle(geometry::triangle3f(leftBottomBack, leftBottomFront, rightBottomFront));
		triangles[ind++].setTextureCoordinates(tcLB, tcLT, tcRT);

		return triangles;
	}
	//----------------------------------------------------------------------------------------------

} /*primitives*/ } /*d3d*/ } /*djah*/