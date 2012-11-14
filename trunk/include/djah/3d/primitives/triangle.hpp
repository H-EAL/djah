#ifndef DJAH_VIDEO_PRIMITIVES_TRIANGLE_HPP
#define DJAH_VIDEO_PRIMITIVES_TRIANGLE_HPP

#include "djah/math.hpp"
#include "djah/geometry/triangle.hpp"

namespace djah { namespace d3d { namespace primitives { 

	class triangle
	{
	public:
		triangle();
		triangle(const math::point3f (&points)[3]);
		triangle(const math::point3f (&points)[3], const math::vector3f (&normals)[3]);
		triangle
		(
			const math::point3f (&points)[3],
			const math::vector3f (&normals)[3],
			const math::point2f (&texCoords)[3]
		);

		triangle(const geometry::triangle3f &model);

		void setPoints(const math::point3f (&points)[3]);
		void setPoints(const math::point3f &pA, const math::point3f &pB, const math::point3f &pC);

		void setNormals(const math::vector3f (&normals)[3]);
		void setNormals(const math::vector3f &nA, const math::vector3f &nB, const math::vector3f &nC);
		void setNormal(const math::vector3f &n);

		void setTextureCoordinates(const math::point2f (&texCoords)[3]);
		void setTextureCoordinates(const math::point2f &tcA, const math::point2f &tcB, const math::point2f &tcC);

		const math::point3f& pointA() const { return vertex_[0].p; }
		const math::point3f& pointB() const { return vertex_[1].p; }
		const math::point3f& pointC() const { return vertex_[2].p; }

		const math::vector3f& normalA() const { return vertex_[0].n; }
		const math::vector3f& normalB() const { return vertex_[1].n; }
		const math::vector3f& normalC() const { return vertex_[2].n; }

		const math::point2f& textureCoordinatesA() const { return vertex_[0].tc; }
		const math::point2f& textureCoordinatesB() const { return vertex_[1].tc; }
		const math::point2f& textureCoordinatesC() const { return vertex_[2].tc; }


		math::point3f& pointA() { return vertex_[0].p; }
		math::point3f& pointB() { return vertex_[1].p; }
		math::point3f& pointC() { return vertex_[2].p; }

		math::vector3f& normalA() { return vertex_[0].n; }
		math::vector3f& normalB() { return vertex_[1].n; }
		math::vector3f& normalC() { return vertex_[2].n; }

		math::point2f& textureCoordinatesA() { return vertex_[0].tc; }
		math::point2f& textureCoordinatesB() { return vertex_[1].tc; }
		math::point2f& textureCoordinatesC() { return vertex_[2].tc; }

	private:
		struct vertex
		{
			math::point3f  p;
			math::vector3f n;
			math::point2f  tc;
		} vertex_[3];
	};

} /*primitives*/ } /*d3d*/ } /*djah*/

#endif /* DJAH_VIDEO_PRIMITIVES_TRIANGLE_HPP */