#include "game/components/visual_mesh.hpp"
#include "game/resources/default_asset_finder.hpp"

namespace game { namespace components {

	//----------------------------------------------------------------------------------------------
	visual_mesh::visual_mesh(const rapidjson::Value &node)
	{
		load_attributes(file);

		spMesh = game::resources::default_asset_finder::get().load<resources::mesh>(file);
	}
	//----------------------------------------------------------------------------------------------
	void visual_mesh::serialize(rapidjson::Document &document, rapidjson::Value &componentNode) const
	{
		save_attributes(file);
	}
	//----------------------------------------------------------------------------------------------

} /*components*/ } /*game*/