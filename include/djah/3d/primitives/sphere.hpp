#ifndef DJAH_3D_PRIMITIVES_SPHERE_HPP
#define DJAH_3D_PRIMITIVES_SPHERE_HPP

#include <vector>
#include "djah/3d/primitives/triangle.hpp"

namespace djah { namespace d3d { namespace primitives {

	class iso_sphere
	{
	public:
		std::vector<triangle> construct(int level1, int level2 = 0);

	private:
		void setTriangle
		(
			triangle &tri,
			const math::vector3f &a,
			const math::vector3f &b,
			const math::vector3f &c
		);

		math::vector2f computeTextureCoordinates
		(
			const math::vector3f &n,
			const math::vector3f &tn
		);
	};


	class sphere
	{
	public:
		std::vector<triangle> construct(int slices = 2, int stacks = 2);

	private:
		void setTriangle
			(
			triangle &tri,
			const math::vector3f &a,
			const math::vector3f &b,
			const math::vector3f &c
			);

		math::vector2f computeTextureCoordinates
			(
			const math::vector3f &n,
			const math::vector3f &tn
			);
	};

} /*primitives*/ } /*d3d*/ } /*djah*/

#endif /* DJAH_3D_PRIMITIVES_SPHERE_HPP */