#ifndef DJAH_3D_PRIMITIVES_RIBBON_HPP
#define DJAH_3D_PRIMITIVES_RIBBON_HPP

#include <vector>
#include "djah/3d/primitives/triangle.hpp"

namespace djah { namespace d3d { namespace primitives {

	class ribbon
	{
	public:
		static std::vector<triangle> construct(int slices = 3, int l = 0);
	};

} /*primitives*/ } /*d3d*/ } /*djah*/

#endif /* DJAH_3D_PRIMITIVES_RIBBON_HPP */
