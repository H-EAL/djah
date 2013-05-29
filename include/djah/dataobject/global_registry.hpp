#ifndef DJAH_DATA_OBJECT_GLOBAL_REGISTRY_HPP
#define DJAH_DATA_OBJECT_GLOBAL_REGISTRY_HPP

#include "../core/typelist.hpp"
#include "../core/singleton.hpp"
#include "registry.hpp"
#include "ini_serializer.hpp"

namespace djah { namespace dataobject {

	//----------------------------------------------------------------------------------------------
	// List of supported types
	typedef TYPELIST
	(
		int, 
		float, 
		bool, 
		double, 
		std::string, 
		math::vector3f, 
		math::quatf
	)
	default_attribute_types;
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename AttributeTypes = default_attribute_types>
	class global_registry
		: public utils::singleton< global_registry<AttributeTypes> >
	{
		DJAH_MAKE_SINGLETON( global_registry<AttributeTypes> );

	public:
		typedef registry<AttributeTypes, ini_serializer>  do_ini_registry_t;
		//typedef registry<AttributeTypes, xml_serializer>  do_xml_registry_t;
		//typedef registry<AttributeTypes, bin_serializer>  do_bin_registry_t;
		//typedef registry<AttributeTypes, json_serializer> do_json_registry_t;

		typedef data_object<AttributeTypes> data_object_t;
		typedef std::shared_ptr<data_object_t> data_object_ptr;

		data_object_ptr getDO(const std::string &dataObjectName)
		{
			return iniRegistry_.get(dataObjectName);
		}

		void reload(data_object_ptr dataObject)
		{
			iniRegistry_.reload(dataObject->name());
		}
		
		void saveDO(data_object_ptr dataObject)
		{
			iniRegistry_.save(dataObject->name());
		}

	private:
		global_registry() {}
		virtual ~global_registry() {}

	private:
		do_ini_registry_t  iniRegistry_;
		//do_xml_registry_t  xmlRegistry_;
		//do_bin_registry_t  binRegistry_;
		//do_json_registry_t jsonRegistry_;
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	typedef global_registry<> default_registry;
	//----------------------------------------------------------------------------------------------

} /*dataobject*/ } /*djah*/

#endif /* DJAH_DATA_OBJECT_GLOBAL_REGISTRY_HPP */