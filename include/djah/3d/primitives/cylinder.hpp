#ifndef DJAH_3D_PRIMITIVES_CYLINDER_HPP
#define DJAH_3D_PRIMITIVES_CYLINDER_HPP

#include <vector>
#include "djah/3d/primitives/triangle.hpp"

namespace djah { namespace d3d { namespace primitives {

	class cylinder
	{
	public:
		static std::vector<triangle> construct(int level = 3, int level2 = 0);
	};

} /*primitives*/ } /*d3d*/ } /*djah*/

#endif /* DJAH_3D_PRIMITIVES_CYLINDER_HPP */
