#ifndef DJAH_GAMEPLAY_COMPONENTS_VISUAL_MESH_HPP
#define DJAH_GAMEPLAY_COMPONENTS_VISUAL_MESH_HPP

#include "djah/gameplay/components/component.hpp"
#include "djah/resources/mesh.hpp"

namespace djah { namespace gameplay { namespace components {

	struct visual_mesh
	{
		MAKE_COMPONENT(visual_mesh);
		
		// serializable
		std::string file;

		// transient
		resources::mesh_sptr spMesh;
	};

} /*components*/ } /*gameplay*/ } /*djah*/

#endif /* DJAH_GAMEPLAY_COMPONENTS_VISUAL_MESH_HPP */