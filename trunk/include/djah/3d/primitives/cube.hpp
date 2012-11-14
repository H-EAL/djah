#ifndef DJAH_3D_PRIMITIVES_CUBE_HPP
#define DJAH_3D_PRIMITIVES_CUBE_HPP

#include <vector>
#include "djah/3d/primitives/triangle.hpp"

namespace djah { namespace d3d { namespace primitives {

	class cube
	{
	public:
		static std::vector<triangle> construct(int);
	};

} /*primitives*/ } /*d3d*/ } /*djah*/

#endif /* DJAH_3D_PRIMITIVES_CUBE_HPP */
