#ifndef DJAH_3D_PRIMITIVES_CONE_HPP
#define DJAH_3D_PRIMITIVES_CONE_HPP

#include <vector>
#include "djah/3d/primitives/triangle.hpp"

namespace djah { namespace d3d { namespace primitives {

	class cone
	{
	public:
		static std::vector<triangle> construct(int level = 3);
	};

} /*primitives*/ } /*d3d*/ } /*djah*/

#endif /* DJAH_3D_PRIMITIVES_CONE_HPP */
