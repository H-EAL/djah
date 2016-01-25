#include "game/components/scene_node.hpp"

namespace game { namespace components {

	//----------------------------------------------------------------------------------------------
    scene_node::scene_node(const rapidjson::Value &node)
	{
		load_attributes( parentIndex );

        in_World.toIdentity();
	}
	//----------------------------------------------------------------------------------------------
    void scene_node::serialize(rapidjson::Document &document, rapidjson::Value &componentNode) const
    {
        save_attributes(parentIndex);
	}
	//----------------------------------------------------------------------------------------------

} /*components*/ } /*game*/