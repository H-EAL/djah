#ifndef DJAH_RESOURCES_LOADERS_HPP
#define DJAH_RESOURCES_LOADERS_HPP

#define DJAH_DECLARE_LOADER(ASSET_TYPE)\
	template<>\
class loader<ASSET_TYPE>\
	: public ASSET_TYPE## _loader\
{ typedef ASSET_TYPE AssetType; };

#define DJAH_DECLARE_TEMPLATED_LOADER(ASSET_TYPE, TEMPLATE_PARAMS)\
	template<>\
class loader<ASSET_TYPE<TEMPLATE_PARAMS>>\
	: public ASSET_TYPE## _loader<TEMPLATE_PARAMS>\
{ typedef ASSET_TYPE<TEMPLATE_PARAMS> AssetType; };

#include "djah/resources/image_loader.hpp"
#include "djah/resources/mesh_loader.hpp"
#include "djah/resources/data_object_loader.hpp"

namespace djah { namespace resources {

	template<typename T>
	class loader;

	DJAH_DECLARE_LOADER(image);
	DJAH_DECLARE_LOADER(mesh);
	DJAH_DECLARE_TEMPLATED_LOADER(data_object, DefaultAttributeTypes);

} /*resources*/ } /*djah*/

#undef DJAH_DECLARE_LOADER

#endif /* DJAH_RESOURCES_LOADER_HPP */