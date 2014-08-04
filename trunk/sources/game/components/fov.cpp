#include "djah/game/components/fov.hpp"

namespace djah { namespace game { namespace components {

	//----------------------------------------------------------------------------------------------
	fov::fov(const rapidjson::Value &node)
	{
		load_attributes
		(
			  horizontal
			, vertical
			, nearPlane
			, farPlane
			, distortion
		);
	}
	//----------------------------------------------------------------------------------------------
	void fov::serialize(rapidjson::Document &doc) const
	{
		save_attributes
		(
			  horizontal
			, vertical
			, nearPlane
			, farPlane
			, distortion
		);
	}
	//----------------------------------------------------------------------------------------------

} /*components*/ } /*game*/ } /*djah*/