#include "djah/game/components/visual_mesh.hpp"
#include "djah/resources/asset_finder.hpp"

namespace djah { namespace game { namespace components {

	//----------------------------------------------------------------------------------------------
	visual_mesh::visual_mesh(const rapidjson::Value &node)
	{
		load_attributes(file);

		spMesh = resources::default_asset_finder::get().load<resources::mesh>(file);
	}
	//----------------------------------------------------------------------------------------------
	void visual_mesh::serialize(rapidjson::Document &document, rapidjson::Value &componentNode) const
	{
		save_attributes(file);
	}
	//----------------------------------------------------------------------------------------------

} /*components*/ } /*game*/ } /*djah*/