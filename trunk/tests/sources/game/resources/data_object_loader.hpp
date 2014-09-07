#ifndef GAME_RESOURCES_DATA_OBJECT_LOADER_HPP
#define GAME_RESOURCES_DATA_OBJECT_LOADER_HPP

#include "djah/filesystem/stream.hpp"

#include "game/resources/ini_serializer.hpp"
#include "game/resources/default_data_object.hpp"


namespace game { namespace resources {
    
	class data_object_loader
	{
	public:
        static bool loadFromStream(djah::filesystem::stream &stream, const std::string &url, data_object_sptr &spDataObject)
		{
			if( !spDataObject )
			{
				spDataObject = std::make_shared<data_object>(url);
			}

			bool success = false;

            const std::string &extension = djah::string_utils::to_lower_case(djah::string_utils::get_file_extension(url));
			if( extension == "config" )
			{
                success = ini_serializer<DefaultAttributeTypes>::deserialize(stream, spDataObject);
			}
			else if( extension == "json" )
			{
                success = ini_serializer<DefaultAttributeTypes>::deserialize2(stream, spDataObject);
			}

			if( !success )
			{
				DJAH_GLOBAL_ERROR()
					<< "registry<>::load("
					<< url
					<< ") : unable to deserialize data_object"
					<< DJAH_END_LOG();
			}

			return success;
		}
	};

} /*resources*/ } /*game*/

#endif /* GAME_RESOURCES_DATA_OBJECT_LOADER_HPP */