namespace djah { namespace dataobject {

	//----------------------------------------------------------------------------------------------
	template<typename AttributeTypes, template<typename> class Serializer>
	inline bool registry<AttributeTypes,Serializer>::has(const std::string &dataObjectName) const
	{
		return dataObjects_.find(dataObjectName) != dataObjects_.end();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename AttributeTypes, template<typename> class Serializer>
	typename registry<AttributeTypes,Serializer>::data_object_ptr
	registry<AttributeTypes,Serializer>::get(const std::string &dataObjectName)
	{
		data_object_ptr dobj;

		auto it = dataObjects_.find(dataObjectName);
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
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename AttributeTypes, template<typename> class Serializer>
	void registry<AttributeTypes,Serializer>::reload(const std::string &dataObjectName)
	{
		auto it = dataObjects_.find(dataObjectName);
		if( it != dataObjects_.end() )
		{
			data_object_ptr newDataObject = load(dataObjectName);
			if( newDataObject )
			{
				data_object_ptr oldDataObject = it->second;
				(*oldDataObject) = (*newDataObject);
			}
		}
		else
		{
			DJAH_BEGIN_LOG(warning)
				<< "registry<>::reload("
				<< dataObjectName
				<< ") : can't reload what has never been loaded"
				<< DJAH_END_LOG();
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename AttributeTypes, template<typename> class Serializer>
	void registry<AttributeTypes,Serializer>::save(const std::string &dataObjectName)
	{
		auto it = dataObjects_.find(dataObjectName);
		if( it != dataObjects_.end() )
		{
			filesystem::stream_ptr strm = filesystem::browser::get().openWriteStream(dataObjectName);
			if( strm )
			{
				Serializer<AttributeTypes>::serialize(strm, it->second);
			}
			else
			{
				DJAH_BEGIN_LOG(warning)
					<< "registry<>::save("
					<< dataObjectName
					<< ") : unable to open stream"
					<< DJAH_END_LOG();
			}
		}
		else
		{
			DJAH_BEGIN_LOG(warning)
				<< "registry<>::save("
				<< dataObjectName
				<< ") : can't save what has never been loaded"
				<< DJAH_END_LOG();
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename AttributeTypes, template<typename> class Serializer>
	typename registry<AttributeTypes,Serializer>::data_object_ptr
	registry<AttributeTypes,Serializer>::load(const std::string &dataObjectName) const
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
				DJAH_BEGIN_LOG(warning)
					<< "registry<>::load("
					<< dataObjectName
					<< ") : unable to deserialize data_object"
					<< DJAH_END_LOG();
			}
		}
		else
		{
			DJAH_BEGIN_LOG(warning)
				<< "registry<>::load("
				<< dataObjectName
				<< ") : unable to open stream"
				<< DJAH_END_LOG();
		}

		return dataObject;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename AttributeTypes, template<typename> class Serializer>
	inline void registry<AttributeTypes,Serializer>::add(data_object_ptr dataObject)
	{
		dataObjects_.insert( data_object_list_t::value_type(dataObject->name(), dataObject) );
	}
	//----------------------------------------------------------------------------------------------

} /*dataobject*/ } /*djah*/