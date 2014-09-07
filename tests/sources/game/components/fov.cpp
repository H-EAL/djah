#include "game/components/fov.hpp"

namespace game { namespace components {

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
	void fov::serialize(rapidjson::Document &document, rapidjson::Value &componentNode) const
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

} /*components*/ } /*game*/