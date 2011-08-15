#ifndef COLLADA_DATABASE_HPP
#define COLLADA_DATABASE_HPP

#include <string>

class TiXmlElement;

namespace collada {

	// Forward declarations
	class proxy;
	namespace library {
		class animations;
		class images;
		class materials;
		class effects;
		class geometries;
		class controllers;
		class visual_scenes;
	}

	class database
	{
		friend class proxy;

	public:
		static database* load(const std::string &filename);
		database();
		~database();

	protected:
		void deserialize(const TiXmlElement &element);

	private:
		library::animations		*lib_animations_;
		library::images			*lib_images_;
		library::materials		*lib_materials_;
		library::effects		*lib_effects_;
		library::geometries		*lib_geometries_;
		library::controllers	*lib_controllers_;
		library::visual_scenes	*lib_visual_scenes_;
	};

} /*collada*/

#endif /* COLLADA_DATABASE_HPP */