#ifndef DJAH_DATA_OBJECT_REGISTRY_HPP
#define DJAH_DATA_OBJECT_REGISTRY_HPP

#include <map>
#include <memory>
#include "../filesystem/browser.hpp"
#include "data_object.hpp"

namespace djah { namespace dataobject {

	//----------------------------------------------------------------------------------------------
	template<typename AttributeTypes, template<typename> class Serializer>
	class registry
	{
	private:
		typedef typename data_object<AttributeTypes> data_object_t;

	public:
		typedef std::shared_ptr<data_object_t> data_object_ptr;

	public:
		data_object_ptr get(const std::string &dataObjectName)
		{
			data_object_ptr dobj;

			data_object_list_const_iterator it = dataObjects_.find(dataObjectName);
			if( it != dataObjects_.end() )
			{
				dobj = it->second;
			}
			else
			{
				dobj = load(dataObjectName);
				if( dobj )
				{
					add(dobj);
				}
			}

			return dobj;
		}

		void reload(const std::string &dataObjectName)
		{
			data_object_list_const_iterator it = dataObjects_.find(dataObjectName);
			if( it != dataObjects_.end() )
			{
				data_object_ptr oldDataObject = it->second;
				data_object_ptr newDataObject = load(dataObjectName);
				if( newDataObject )
				{
					(*oldDataObject) = (*newDataObject);
				}
			}
		}

	private:
		data_object_ptr load(const std::string &dataObjectName)
		{
			data_object_ptr dataObject;

			filesystem::stream_ptr strm = filesystem::browser::get().openReadStream(dataObjectName);
			if( strm )
			{
				data_object_ptr dobjTmp( new data_object_t(dataObjectName) );

				if( Serializer<AttributeTypes>::deserialize(strm, dobjTmp) )
				{
					dataObject = dobjTmp;
				}
				else
				{
					// ERROR
				}
			}

			return dataObject;
		}

		void add(data_object_ptr dataObject)
		{
			assert(dataObject);
			dataObjects_.insert( data_object_list_t::value_type(dataObject->name(), dataObject) );
		}

	private:
		typedef std::map<std::string, data_object_ptr> data_object_list_t;
		typedef typename data_object_list_t::const_iterator data_object_list_const_iterator;

		data_object_list_t dataObjects_;
	};
	//----------------------------------------------------------------------------------------------

} /*dataobject*/ } /*djah*/

#endif /* DJAH_DATA_OBJECT_REGISTRY_HPP */