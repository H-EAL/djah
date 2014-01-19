#include "djah/gameplay/components/transform.hpp"

namespace djah { namespace gameplay { namespace components {

	//----------------------------------------------------------------------------------------------
	transform::transform(const rapidjson::Value &node)
	{
		JSON_READ(position);
		JSON_READ(orientation);
		JSON_READ(scale);
	}
	//----------------------------------------------------------------------------------------------
	void transform::serialize(djah::filesystem::stream &strm) const
	{
		JSON_WRITE(position);
		JSON_WRITE(orientation);
		JSON_WRITE(scale);
	}
	//----------------------------------------------------------------------------------------------

} /*components*/ } /*gameplay*/ } /*djah*/