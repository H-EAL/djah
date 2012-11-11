#ifndef DJAH_VIDEO_PRIMITIVES_SPHERE_HPP
#define DJAH_VIDEO_PRIMITIVES_SPHERE_HPP

#include <vector>
#include "../../math/vector2.hpp"
#include "../../math/vector3.hpp"
#include "../../geometry/triangle.hpp"

namespace djah { namespace video { namespace primitives {

	class sphere
	{
	public:
		template<int Level>
		std::vector<geometry::triangle3f> construct()
		{
			std::vector<geometry::triangle3f> triangles[Level+1];
			int level = Level;

			for(int i = 0; i < Level; ++i)
			{
				const unsigned int nbTriangles = 1 << (3 + (i << 2));
				triangles[i].reserve( nbTriangles  );
			}

			const math::vector3f bottom( 0,-1, 0);
			const math::vector3f top   ( 0, 1, 0);
			const math::vector3f left  (-1, 0, 0);
			const math::vector3f right ( 1, 0, 0);
			const math::vector3f front ( 0, 0, 1);
			const math::vector3f back  ( 0, 0,-1);

			// Top
			triangles[0].push_back( geometry::triangle3f(top, left , front) );
			triangles[0].push_back( geometry::triangle3f(top, front, right) );
			triangles[0].push_back( geometry::triangle3f(top, right, back ) );
			triangles[0].push_back( geometry::triangle3f(top, back , left ) );
			// Bottom
			triangles[0].push_back( geometry::triangle3f(left , bottom, front) );
			triangles[0].push_back( geometry::triangle3f(front, bottom, right) );
			triangles[0].push_back( geometry::triangle3f(right, bottom, back)  );
			triangles[0].push_back( geometry::triangle3f(back , bottom, left)  );

			for(int i = 1; i <= Level; ++i)
			{
				auto itEnd = triangles[i-1].end();
				for(auto it = triangles[i-1].begin(); it != itEnd; ++it)
				{
					const geometry::triangle3f &currentTriangle = *it;

					const math::vector3f &ab = ((currentTriangle.a + currentTriangle.b) / 2.0f).normalize();
					const math::vector3f &bc = ((currentTriangle.b + currentTriangle.c) / 2.0f).normalize();
					const math::vector3f &ca = ((currentTriangle.c + currentTriangle.a) / 2.0f).normalize();

					triangles[i].push_back( geometry::triangle3f(currentTriangle.a, ab, ca) );
					triangles[i].push_back( geometry::triangle3f(ab, currentTriangle.b, bc) );
					triangles[i].push_back( geometry::triangle3f(bc, currentTriangle.c, ca) );
					triangles[i].push_back( geometry::triangle3f(ab, bc, ca)                );
				}
			}

			return triangles[Level];
		}

	private:
	};

} /*primitives*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_PRIMITIVES_SPHERE_HPP */