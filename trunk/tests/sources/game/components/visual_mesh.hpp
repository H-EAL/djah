#ifndef GAME_COMPONENTS_VISUAL_MESH_HPP
#define GAME_COMPONENTS_VISUAL_MESH_HPP

#include "djah/gameplay/component.hpp"
#include "game/resources/mesh.hpp"

namespace game { namespace components {

	struct visual_mesh
	{
		MAKE_COMPONENT(visual_mesh);
		
		// serializable
		std::string file;

		// transient
		resources::mesh_sptr spMesh;
	};

} /*components*/ } /*game*/

#endif /* GAME_COMPONENTS_VISUAL_MESH_HPP */