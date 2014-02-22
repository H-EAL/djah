namespace djah { namespace resources {

	//----------------------------------------------------------------------------------------------
	template<typename ExtraAssetsTypes_, bool UseDefaultTypes_>
	template<typename T>
	void asset_finder<ExtraAssetsTypes_, UseDefaultTypes_>::registerExtensions(const std::string &ext)
	{
		string_utils::string_list_t extensions;
		string_utils::split_string(ext, extensions, " /\\*.,;|-_\t\n'\"");

		auto itEnd = extensions.end();
		for(auto it = extensions.begin(); it != itEnd; ++it)
		{
			const std::string &extension = string_utils::to_lower_case(*it);
			loader_holder<T>::extensions_.insert( extension );
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename ExtraAssetsTypes_, bool UseDefaultTypes_>
	template<typename T>
	std::shared_ptr<T> asset_finder<ExtraAssetsTypes_, UseDefaultTypes_>::get(const std::string &url, bool loadIfNotFound)
	{
		// Get it from the warehouse
		auto pAsset = asset_warehouse::get().find<T>(url);

		// If found check if there's a more up to date version
		if( pAsset )
		{
			filesystem::stream_sptr pStream = filesystem::browser::get().openReadStream(url);
			if( pStream )
			{
				const u64 actualTimestamp = pStream->lastWrite();
				const u64 currentTimestamp = pAsset->timestamp();
				if( actualTimestamp != currentTimestamp )
				{
					std::shared_ptr<T> pReloadedAsset = loadFromUrl<T>(url);
					if( pReloadedAsset )
					{
						*pAsset = *pReloadedAsset;
					}
				}
			}
		}

		// If not found try to load it through the asset manager
		else if( !pAsset && loadIfNotFound )
		{
			pAsset = loadFromUrl<T>(url);
			if( pAsset )
			{
				asset_warehouse::get().add(url, pAsset);
			}
			else
			{
				DJAH_GLOBAL_CRITICAL()
					<< "[AssetFinder] Unable to find the following resource: "
					<< url
					<< DJAH_END_LOG();
			}
		}

		return pAsset;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename ExtraAssetsTypes_, bool UseDefaultTypes_>
	template<typename T>
	std::shared_ptr<T> asset_finder<ExtraAssetsTypes_, UseDefaultTypes_>::loadFromUrl(const std::string &url)
	{
		std::shared_ptr<T> pAsset;

		if( hasLoader<T>(url) )
		{
			filesystem::stream_sptr pStream = filesystem::browser::get().openReadStream(url);
			if(pStream)
			{
				pAsset = loader<T>::loadFromStream(*pStream, url);
				if( pAsset )
				{
					pAsset->setTimestamp(pStream->lastWrite());
				}
			}
		}

		return pAsset;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename ExtraAssetsTypes_, bool UseDefaultTypes_>
	template<typename T>
	void asset_finder<ExtraAssetsTypes_, UseDefaultTypes_>::saveToUrl(const T &obj, const std::string &url)
	{
		if( hasLoader<T>(url) )
		{
			filesystem::stream_sptr pStream = filesystem::browser::get().openWriteStream(url);
			if(pStream)
			{
				loader<T>::saveToStream(obj, pStream, url);
			}
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename ExtraAssetsTypes_, bool UseDefaultTypes_>
	template<typename T>
	bool asset_finder<ExtraAssetsTypes_, UseDefaultTypes_>::hasLoader(const std::string &url)
	{
		std::string extension = string_utils::get_file_extension(url);
		extension = string_utils::to_lower_case(extension);

		auto it = loader_holder<T>::extensions_.find(extension);
		return it != loader_holder<T>::extensions_.end();
	}
	//----------------------------------------------------------------------------------------------

} /*resources*/ } /*djah*/