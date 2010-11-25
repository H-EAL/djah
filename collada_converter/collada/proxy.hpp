#ifndef COLLADA_PROXY_HPP
#define COLLADA_PROXY_HPP

#include <string>
#include "fwd_decl.hpp"

namespace collada {

	enum E_LIBRARY_NAME
	{
		ELN_ANIMATIONS,
		ELN_IMAGES,
		ELN_MATERIALS,
		ELN_EFFECTS,
		ELN_GEOMETRIES,
		ELN_CONTROLLERS,
		ELN_VISUAL_SCENES,

		ELN_COUNT
	};

	class proxy
	{
	public:
		proxy(const std::string &filename);
		~proxy();

		bool good() const;

		library::animations*	getAnimationsLib()		const;
		library::images*		getImagesLib()			const;
		library::materials*		getMaterialsLib()		const;
		library::effects*		getEffectsLib()			const;
		library::geometries*	getGeometriesLib()		const;
		library::controllers*	getControllersLib()		const;
		library::visual_scenes* getVisualScenesLib()	const;

		const library::animation_list_t&		getAnimations()		const;
		const library::image_list_t&			getImages()			const;
		const library::material_list_t&			getMaterials()		const;
		//const library::effect_list_t&			getEffects()		const;
		const library::geometry_list_t&			getGeometries()		const;
		const library::controller_list_t&		getControllers()	const;
		const library::visual_scene_list_t&		getVisualScenes()	const;

	private:
		database *db_;
	};

} /*collada*/

#endif /* COLLADA_PROXY_HPP */