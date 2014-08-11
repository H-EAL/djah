namespace djah { namespace resources {

	//----------------------------------------------------------------------------------------------
	template<typename ExtraAssetsTypes_, bool UseDefaultTypes_>
	template<typename AssetType>
	void asset_finder<ExtraAssetsTypes_, UseDefaultTypes_>::registerExtensions(const std::string &ext)
	{
		string_utils::string_list_t extensions;
		string_utils::split_string(ext, extensions, " /\\*.,;|-_\t\n'\"");

		auto itEnd = extensions.end();
		for(auto it = extensions.begin(); it != itEnd; ++it)
		{
			const std::string &extension = string_utils::to_lower_case(*it);
			asset_extensions<AssetType>::extensions_.insert( extension );
		}
	}
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	template<typename ExtraAssetsTypes_, bool UseDefaultTypes_>
	void asset_finder<ExtraAssetsTypes_, UseDefaultTypes_>::refreshAll()
	{
		assets_visitor<AssetsTypeList>::refresh(*this);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename ExtraAssetsTypes_, bool UseDefaultTypes_>
	template<typename AssetType>
	void asset_finder<ExtraAssetsTypes_, UseDefaultTypes_>::refresh()
	{
		assets_visitor<TYPELIST(AssetType)>::refresh(*this);
	}
	//---------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename ExtraAssetsTypes_, bool UseDefaultTypes_>
	template<typename AssetType>
	std::shared_ptr<AssetType> asset_finder<ExtraAssetsTypes_, UseDefaultTypes_>::load(const std::string &url, bool loadIfNotFound)
	{
		bool success = false;

		// Get it from the warehouse
		auto spAsset = asset_warehouse::get().find<AssetType>(url);

		// If found check if there's a more up to date version
		if( spAsset )
		{
			success = refresh<AssetType>(url, spAsset);
			if( !success )
			{
				asset_warehouse::get().remove(spAsset);
			}
		}

		// If not found try to load it through the asset manager
		else if( !spAsset && loadIfNotFound )
		{
			success = loadFromUrl<AssetType>(url, spAsset);
			if( success )
			{
				asset_warehouse::get().add(url, spAsset);
			}
		}

		if( !success )
		{
			spAsset.reset();

			DJAH_GLOBAL_CRITICAL()
				<< "[AssetFinder] Unable to find the following resource: "
				<< url
				<< DJAH_END_LOG();
		}

		return spAsset;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename ExtraAssetsTypes_, bool UseDefaultTypes_>
	template<typename AssetType>
	bool asset_finder<ExtraAssetsTypes_, UseDefaultTypes_>::loadFromUrl(const std::string &url, std::shared_ptr<AssetType> &spAsset)
	{
		bool success = false;

		if( hasLoader<AssetType>(url) )
		{
			filesystem::stream_sptr spStream = filesystem::browser::get().openReadStream(url);
			if(spStream)
			{
				success = loader<AssetType>::loadFromStream(*spStream, url, spAsset);
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
	template<typename AssetType>
	bool asset_finder<ExtraAssetsTypes_, UseDefaultTypes_>::refresh(const std::string &url, std::shared_ptr<AssetType> &spAsset)
	{
		bool success = false;

		filesystem::stream_sptr spStream = filesystem::browser::get().openReadStream(url);
		if( spStream )
		{
			const u64 actualTimestamp = spStream->lastWrite();
			const u64 currentTimestamp = spAsset->timestamp();
			if( actualTimestamp == currentTimestamp )
			{
				success = true;
			}
			else
			{
				success = loadFromUrl<AssetType>(url, spAsset);
			}
		}

		return success;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename ExtraAssetsTypes_, bool UseDefaultTypes_>
	template<typename AssetType>
	void asset_finder<ExtraAssetsTypes_, UseDefaultTypes_>::saveToUrl(const AssetType &obj, const std::string &url)
	{
		if( hasLoader<AssetType>(url) )
		{
			filesystem::stream_sptr pStream = filesystem::browser::get().openWriteStream(url);
			if(pStream)
			{
				loader<AssetType>::saveToStream(obj, pStream, url);
			}
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename ExtraAssetsTypes_, bool UseDefaultTypes_>
	template<typename AssetType>
	bool asset_finder<ExtraAssetsTypes_, UseDefaultTypes_>::hasLoader(const std::string &url)
	{
		std::string extension = string_utils::get_file_extension(url);
		extension = string_utils::to_lower_case(extension);

		const auto &it = asset_extensions<AssetType>::extensions_.find(extension);
		return it != asset_extensions<AssetType>::extensions_.end();
	}
	//----------------------------------------------------------------------------------------------

} /*resources*/ } /*djah*/