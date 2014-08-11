#include "djah/game/components/texture.hpp"
#include "../../../tests/sources/resource_finder.hpp"

namespace djah { namespace game { namespace components {

	//----------------------------------------------------------------------------------------------
	texture::texture(const rapidjson::Value &node)
	{
		load_attributes(file);

		spTexture = d3d::texture_manager::get().find(file);
	}
	//----------------------------------------------------------------------------------------------
	void texture::serialize(rapidjson::Document &document, rapidjson::Value &componentNode) const
	{
		save_attributes(file);
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	uv_modifier::uv_modifier(const rapidjson::Value &node)
	{
		load_attributes(scale, offset, velocity, textureFilter);
	}
	//----------------------------------------------------------------------------------------------
	void uv_modifier::serialize(rapidjson::Document &document, rapidjson::Value &componentNode) const
	{
		save_attributes(scale, offset, velocity, textureFilter);
	}
	//----------------------------------------------------------------------------------------------

} /*components*/ } /*game*/ } /*djah*/