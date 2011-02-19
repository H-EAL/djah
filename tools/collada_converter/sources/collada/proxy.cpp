#include "proxy.hpp"
#include "database.hpp"

#include "library/animations.hpp"
#include "library/images.hpp"
#include "library/materials.hpp"
#include "library/effects.hpp"
#include "library/geometries.hpp"
#include "library/controllers.hpp"
#include "library/visual_scenes.hpp"

namespace collada {

	//----------------------------------------------------------------------------------------------
	proxy::proxy(const std::string &filename)
		: db_( database::load(filename) )
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	proxy::~proxy()
	{
		delete db_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool proxy::good() const
	{
		return db_ != 0;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	library::animations*	proxy::getAnimationsLib()	const
	{ return db_->lib_animations_; }
	library::images*		proxy::getImagesLib()		const
	{ return db_->lib_images_; }
	library::materials*		proxy::getMaterialsLib()	const
	{ return db_->lib_materials_; }
	library::effects*		proxy::getEffectsLib()		const
	{ return db_->lib_effects_; }
	library::geometries*	proxy::getGeometriesLib()	const
	{ return db_->lib_geometries_; }
	library::controllers*	proxy::getControllersLib()	const
	{ return db_->lib_controllers_; }
	library::visual_scenes* proxy::getVisualScenesLib()	const
	{ return db_->lib_visual_scenes_; }
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	const library::animation_list_t&		proxy::getAnimations()		const
	{ return getAnimationsLib()->animations_; }
	const library::image_list_t&			proxy::getImages()			const
	{ return getImagesLib()->images_; }
	const library::material_list_t&			proxy::getMaterials()		const
	{ return getMaterialsLib()->materials_; }
	//const library::effect_list_t&			proxy::getEffects()			const
	//{ return getEffectsLib()->effects_; }
	const library::geometry_list_t&			proxy::getGeometries()		const
	{ return getGeometriesLib()->geometries_; }
	const library::controller_list_t&		proxy::getControllers()		const
	{ return getControllersLib()->controllers_; }
	const library::visual_scene_list_t&		proxy::getVisualScenes()	const
	{ return getVisualScenesLib()->visual_scenes_; }
	//----------------------------------------------------------------------------------------------

} /*collada*/