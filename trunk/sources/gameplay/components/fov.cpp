#include "djah/gameplay/components/fov.hpp"

namespace djah { namespace gameplay { namespace components {

	//----------------------------------------------------------------------------------------------
	fov::fov(const rapidjson::Value &node)
	{
		JSON_READ(horizontal);
		JSON_READ(vertical);
		JSON_READ(nearPlane);
		JSON_READ(farPlane);
		JSON_READ(distortion);
	}
	//----------------------------------------------------------------------------------------------
	void fov::serialize(djah::filesystem::stream &strm) const
	{
		JSON_WRITE(horizontal);
		JSON_WRITE(vertical);
		JSON_WRITE(nearPlane);
		JSON_WRITE(farPlane);
		JSON_WRITE(distortion);
	}
	//----------------------------------------------------------------------------------------------

} /*components*/ } /*gameplay*/ } /*djah*/