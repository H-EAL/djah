#ifndef DJAH_GAMEPLAY_COMPONENTS_HPP
#define DJAH_GAMEPLAY_COMPONENTS_HPP

#include "rapidjson/document.h"
#include "djah/math.hpp"
#include "djah/core/typelist.hpp"
#include "djah/filesystem/stream.hpp"

#define MAKE_COMPONENT(COMP) \
	static const char* name() { return #COMP; }\
	void serialize(djah::filesystem::stream &strm) const;\
	void deserialize(const rapidjson::Value &node);\

namespace djah { namespace gameplay { namespace components {

	struct position
	{
		MAKE_COMPONENT(position);
		math::vector3f value;
	};

	struct orientation
	{
		MAKE_COMPONENT(orientation);
		math::quatf value;
	};

	struct fov
	{
		MAKE_COMPONENT(fov);
		float horizontal;
		float vertical;
	};

	typedef TYPELIST
	(
		position,
		orientation,
		fov
	)
	default_components_tl;

} /*components*/ } /*gameplay*/ } /*djah*/

#endif /* DJAH_GAMEPLAY_COMPONENTS_HPP */