#include "djah/gameplay/components/transform.hpp"

namespace djah { namespace gameplay { namespace components {

	//----------------------------------------------------------------------------------------------
	transform::transform(const rapidjson::Value &node)
	{
		load_attributes
		(
			  position
			, orientation
			, scale
		);
	}
	//----------------------------------------------------------------------------------------------
	void transform::serialize(rapidjson::Document &doc) const
	{
		save_attributes
		(
			  position
			, orientation
			, scale
		);
	}
	//----------------------------------------------------------------------------------------------

} /*components*/ } /*gameplay*/ } /*djah*/