#ifndef DJAH_GAME_COMPONENTS_VISUAL_MESH_HPP
#define DJAH_GAME_COMPONENTS_VISUAL_MESH_HPP

#include "djah/gameplay/component.hpp"
#include "djah/resources/mesh.hpp"

namespace djah { namespace game { namespace components {

	struct visual_mesh
	{
		MAKE_COMPONENT(visual_mesh);
		
		// serializable
		std::string file;

		// transient
		resources::mesh_sptr spMesh;
	};

} /*components*/ } /*game*/ } /*djah*/

#endif /* DJAH_GAME_COMPONENTS_VISUAL_MESH_HPP */