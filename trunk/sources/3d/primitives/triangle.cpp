#include "djah/3d/primitives/triangle.hpp"

namespace djah { namespace d3d { namespace primitives {

	//----------------------------------------------------------------------------------------------
	triangle::triangle()
	{

	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	triangle::triangle(const math::point3f (&points)[3])
	{
		setPoints(points);

		geometry::triangle3f model(points[0], points[1], points[2]);
		setNormal( model.normal() );
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	triangle::triangle(const math::point3f (&points)[3], const math::vector3f (&normals)[3])
	{
		setPoints(points);
		setNormals(normals);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	triangle::triangle
	(
		const math::point3f (&points)[3],
		const math::vector3f (&normals)[3],
		const math::point2f (&texCoords)[3]
	)
	{
		setPoints(points);
		setNormals(normals);
		setTextureCoordinates(texCoords);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	triangle::triangle(const geometry::triangle3f &model)
	{
		setPoints(model.a, model.b, model.c);
		setNormal( model.normal() );
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void triangle::setPoints(const math::point3f (&points)[3])
	{
		setPoints(points[0], points[1], points[2]);
	}
	//----------------------------------------------------------------------------------------------
	void triangle::setPoints(const math::point3f &pA, const math::point3f &pB, const math::point3f &pC)
	{
		vertex_[0].p = pA;
		vertex_[1].p = pB;
		vertex_[2].p = pC;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void triangle::setNormals(const math::vector3f (&normals)[3])
	{
		setNormals(normals[0], normals[1], normals[2]);
	}
	//----------------------------------------------------------------------------------------------
	void triangle::setNormals(const math::vector3f &nA, const math::vector3f &nB, const math::vector3f &nC)
	{
		vertex_[0].n = nA;
		vertex_[1].n = nB;
		vertex_[2].n = nC;
	}
	//----------------------------------------------------------------------------------------------
	void triangle::setNormal(const math::vector3f &n)
	{
		vertex_[0].n = n;
		vertex_[1].n = n;
		vertex_[2].n = n;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void triangle::setTextureCoordinates(const math::point2f (&texCoords)[3])
	{
		setTextureCoordinates(texCoords[0], texCoords[1], texCoords[2]);
	}
	//----------------------------------------------------------------------------------------------
	void triangle::setTextureCoordinates(const math::point2f &tcA, const math::point2f &tcB, const math::point2f &tcC)
	{
		vertex_[0].tc = tcA;
		vertex_[1].tc = tcB;
		vertex_[2].tc = tcC;
	}
	//----------------------------------------------------------------------------------------------

} /*primitives*/ } /*d3d*/ } /*djah*/