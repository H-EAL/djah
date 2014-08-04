#include "djah/game/components/transform.hpp"

namespace djah { namespace game { namespace components {

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

} /*components*/ } /*game*/ } /*djah*/