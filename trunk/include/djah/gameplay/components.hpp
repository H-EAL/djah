#ifndef DJAH_GAMEPLAY_COMPONENTS_HPP
#define DJAH_GAMEPLAY_COMPONENTS_HPP

#include <string>
#include "rapidjson/document.h"
#include "djah/math.hpp"
#include "djah/core/typelist.hpp"
#include "djah/filesystem/stream.hpp"

#define MAKE_COMPONENT(COMP) \
	static const char* name() { return #COMP; }\
	void serialize(djah::filesystem::stream &strm) const;\
	void deserialize(const rapidjson::Value &node);\

namespace djah { namespace gameplay { namespace components {

	struct transform
	{
		MAKE_COMPONENT(transform);
		math::vector3f position;
		math::vector3f orientation;
		math::vector3f scale;
	};

	struct fov
	{
		MAKE_COMPONENT(fov);
		float horizontal;
		float vertical;
	};

	struct static_mesh
	{
		MAKE_COMPONENT(static_mesh);
		std::string file;
	};

	struct texture
	{
		MAKE_COMPONENT(texture);
		std::string file;
	};

	typedef TYPELIST
	(
		transform,
		fov,
		static_mesh,
		texture
	)
	default_components_tl;

} /*components*/ } /*gameplay*/ } /*djah*/

#endif /* DJAH_GAMEPLAY_COMPONENTS_HPP */