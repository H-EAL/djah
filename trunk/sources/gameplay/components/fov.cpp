#include "djah/gameplay/components/fov.hpp"

namespace djah { namespace gameplay { namespace components {

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

} /*components*/ } /*gameplay*/ } /*djah*/