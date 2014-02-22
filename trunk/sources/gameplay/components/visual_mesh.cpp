#include "djah/gameplay/components/visual_mesh.hpp"

namespace djah { namespace gameplay { namespace components {

	//----------------------------------------------------------------------------------------------
	visual_mesh::visual_mesh(const rapidjson::Value &node)
	{
		load_attributes(file);

		//spMesh = asset_finder::get<resources::mesh>(file)
	}
	//----------------------------------------------------------------------------------------------
	void visual_mesh::serialize(rapidjson::Document &doc) const
	{
		save_attributes(file);
	}
	//----------------------------------------------------------------------------------------------

} /*components*/ } /*gameplay*/ } /*djah*/