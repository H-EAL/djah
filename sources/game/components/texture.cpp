#include "djah/game/components/texture.hpp"

namespace djah { namespace game { namespace components {

	//----------------------------------------------------------------------------------------------
	texture::texture(const rapidjson::Value &node)
	{
		load_attributes(file);
	}
	//----------------------------------------------------------------------------------------------
	void texture::serialize(rapidjson::Document &doc) const
	{
		save_attributes(file);
	}
	//----------------------------------------------------------------------------------------------

} /*components*/ } /*game*/ } /*djah*/