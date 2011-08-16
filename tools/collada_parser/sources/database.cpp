#include "database.hpp"

#include <tinyxml.h>

#include "library/animations.hpp"
#include "library/images.hpp"
#include "library/materials.hpp"
#include "library/effects.hpp"
#include "library/geometries.hpp"
#include "library/controllers.hpp"
#include "library/visual_scenes.hpp"

namespace collada {

	//----------------------------------------------------------------------------------------------
	database* database::load(const std::string &filename)
	{
		database *new_db = 0;

		TiXmlDocument doc(filename.c_str());
		if( doc.LoadFile() )
		{
			new_db = new database;
			TiXmlElement *root_element = doc.RootElement();
			new_db->deserialize(*root_element);
		}

		return new_db;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	database::database()
		: lib_animations_(0)
		, lib_images_(0)
		, lib_materials_(0)
		, lib_effects_(0)
		, lib_geometries_(0)
		, lib_controllers_(0)
		, lib_visual_scenes_(0)
	{
	}
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	template<typename LIB>
	void deserialize_library(const TiXmlElement &element, const std::string &lib_name, LIB *&lib)
	{
		std::string lib_str = "library_" + lib_name;
		const TiXmlElement *lib_elem = element.FirstChildElement(lib_str.c_str());
		if( lib_elem )
		{
			lib = new LIB;
			lib->deserialize(*lib_elem);
		}
	}
	//----------------------------------------------------------------------------------------------
	void database::deserialize(const TiXmlElement &element)
	{
		deserialize_library(element, "animations",		lib_animations_);
		deserialize_library(element, "images",			lib_images_);
		deserialize_library(element, "materials",		lib_materials_);
		deserialize_library(element, "effects",			lib_effects_);
		deserialize_library(element, "geometries",		lib_geometries_);
		deserialize_library(element, "controllers",		lib_controllers_);
		deserialize_library(element, "visual_scenes",	lib_visual_scenes_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	database::~database()
	{
		delete lib_animations_;
		delete lib_images_;
		delete lib_materials_;
		delete lib_effects_;
		delete lib_geometries_;
		delete lib_controllers_;
		delete lib_visual_scenes_;
	}
	//----------------------------------------------------------------------------------------------

} /*collada*/