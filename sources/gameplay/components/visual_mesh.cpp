#include "djah/gameplay/components/visual_mesh.hpp"

namespace djah { namespace gameplay { namespace components {

	//----------------------------------------------------------------------------------------------
	visual_mesh::visual_mesh(const rapidjson::Value &node)
	{
		JSON_READ(file);

		//spMesh = asset_finder::get<resources::mesh>(file)
	}
	//----------------------------------------------------------------------------------------------
	void visual_mesh::serialize(djah::filesystem::stream &strm) const
	{
		JSON_WRITE(file);
	}
	//----------------------------------------------------------------------------------------------

} /*components*/ } /*gameplay*/ } /*djah*/