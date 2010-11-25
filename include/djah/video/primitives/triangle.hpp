#ifndef DJAH_VIDEO_PRIMITIVES_HPP
#define DJAH_VIDEO_PRIMITIVES_HPP

#include "../../math.hpp"
#include "../../geometry/triangle.hpp"

namespace djah { namespace video { namespace primitives { 

	class triangle
	{
	public:

		triangle(const math::point3f (&points)[3]);
		triangle(const math::point3f (&points)[3], const math::vector3f (&normals)[3]);
		triangle(const math::point3f (&points)[3], const math::vector3f (&normals)[3], const math::point2f (&texCoords)[3]);
		triangle(const geometry::triangle_f &model);

		void setPoints(const math::point3f (&points)[3]);
		void setNormals(const math::vector3f (&normals)[3]);
		void setTextureCoordinates(const math::point2f (&texCoords)[3]);

	private:

		math::point3f	points_[3];
		math::vector3f	normals_[3];
		math::point2f	tex_coords_[3];
	};

} /*primitives*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_PRIMITIVES_HPP */