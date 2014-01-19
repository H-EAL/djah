#include "djah/gameplay/components/texture.hpp"

namespace djah { namespace gameplay { namespace components {

	//----------------------------------------------------------------------------------------------
	texture::texture(const rapidjson::Value &node)
	{
		JSON_READ(file);
	}
	//----------------------------------------------------------------------------------------------
	void texture::serialize(djah::filesystem::stream &strm) const
	{
		JSON_WRITE(file);
	}
	//----------------------------------------------------------------------------------------------

} /*components*/ } /*gameplay*/ } /*djah*/