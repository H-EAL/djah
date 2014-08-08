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
			asset_extensions<T>::extensions_.insert( extension );
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename ExtraAssetsTypes_, bool UseDefaultTypes_>
	template<typename T>
	std::shared_ptr<T> asset_finder<ExtraAssetsTypes_, UseDefaultTypes_>::load(const std::string &url, bool loadIfNotFound)
	{
		// Get it from the warehouse
		auto spAsset = asset_warehouse::get().find<T>(url);

		// If found check if there's a more up to date version
		if( spAsset )
		{
			filesystem::stream_sptr spStream = filesystem::browser::get().openReadStream(url);
			if( spStream )
			{
				const u64 actualTimestamp = spStream->lastWrite();
				const u64 currentTimestamp = spAsset->timestamp();
				if( actualTimestamp != currentTimestamp )
				{
					const bool success = loadFromUrl<T>(url, spAsset);
					if( !success )
					{
						asset_warehouse::get().remove(spAsset);
						spAsset.reset();
					}
				}
			}
		}

		// If not found try to load it through the asset manager
		else if( !spAsset && loadIfNotFound )
		{
			const bool success = loadFromUrl<T>(url, spAsset);
			if( success )
			{
				asset_warehouse::get().add(url, spAsset);
			}
			else
			{
				DJAH_GLOBAL_CRITICAL()
					<< "[AssetFinder] Unable to find the following resource: "
					<< url
					<< DJAH_END_LOG();
			}
		}

		return spAsset;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename ExtraAssetsTypes_, bool UseDefaultTypes_>
	template<typename T>
	bool asset_finder<ExtraAssetsTypes_, UseDefaultTypes_>::loadFromUrl(const std::string &url, std::shared_ptr<T> &spAsset)
	{
		bool success = false;

		if( hasLoader<T>(url) )
		{
			filesystem::stream_sptr spStream = filesystem::browser::get().openReadStream(url);
			if(spStream)
			{
				success = loader<T>::loadFromStream(*spStream, url, spAsset);
				if( success )
				{
					spAsset->setTimestamp(spStream->lastWrite());
				}
			}
		}

		return success;
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

		auto it = asset_extensions<T>::extensions_.find(extension);
		return it != asset_extensions<T>::extensions_.end();
	}
	//----------------------------------------------------------------------------------------------

} /*resources*/ } /*djah*/