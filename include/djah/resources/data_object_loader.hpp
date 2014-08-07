#ifndef DJAH_RESOURCES_DATAOBJECT_LOADER_HPP
#define DJAH_RESOURCES_DATAOBJECT_LOADER_HPP

#include "djah/filesystem/stream.hpp"
#include "djah/resources/data_object.hpp"
#include "djah/resources/ini_serializer.hpp"

namespace djah { namespace resources {

	template<typename AttributeTypes>
	class data_object_loader
	{
	public:
		typedef typename data_object<AttributeTypes>::data_object_t	   data_object_t;
		typedef typename data_object<AttributeTypes>::data_object_sptr data_object_sptr;

	public:
		static data_object_sptr loadFromStream(filesystem::stream &stream, const std::string &url = "")
		{
			data_object_sptr pDataObject = std::make_shared<data_object_t>(url);
			data_object_sptr pLoadedDataObject = std::make_shared<data_object_t>(url);

			bool success = false;
			const std::string &extension = string_utils::to_lower_case(string_utils::get_file_extension(url));
			if( extension == "config" )
			{
				if( success = ini_serializer<AttributeTypes>::deserialize(stream, pLoadedDataObject) )
				{
					pDataObject = pLoadedDataObject;
				}
			}
			else if( extension == "json" )
			{
				if( success = ini_serializer<AttributeTypes>::deserialize2(stream, pLoadedDataObject) )
				{
					pDataObject = pLoadedDataObject;
				}
			}

			if( !success )
			{
				DJAH_GLOBAL_ERROR()
					<< "registry<>::load("
					<< url
					<< ") : unable to deserialize data_object"
					<< DJAH_END_LOG();
			}

			return pDataObject;
		}
	};

} /*resources*/ } /*djah*/

#endif /* DJAH_RESOURCES_DATAOBJECT_LOADER_HPP */