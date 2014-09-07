#include "game/components/texture.hpp"
#include "../../resource_finder.hpp"

namespace game { namespace components {

	//----------------------------------------------------------------------------------------------
	texture::texture(const rapidjson::Value &node)
	{
		load_attributes(file);

        spTexture = djah::d3d::texture_manager::get().find(file);
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

} /*components*/ } /*game*/