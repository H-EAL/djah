namespace djah { namespace resources {

	//----------------------------------------------------------------------------------------------
	template<typename ExtraAssetsTypes_, bool UseDefaultTypes_>
	template<typename T>
	void asset_finder<ExtraAssetsTypes_, UseDefaultTypes_>::registerLoader(std::shared_ptr<loader<T>> pLoader, const std::string &ext)
	{
		DJAH_LOG_TODO("Use loaders as  static classes");

		std::vector<std::string> extensions;
		utils::split_string(ext, extensions, " /\\*.,;|-_\t\n'\"");

		auto itEnd = extensions.end();
		for(auto it = extensions.begin(); it != itEnd; ++it)
		{
			const std::string &extension = utils::to_lower_case(*it);
			loader_holder<T>::loaders_[extension] = pLoader;
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename ExtraAssetsTypes_, bool UseDefaultTypes_>
	template<typename T>
	std::shared_ptr<T> asset_finder<ExtraAssetsTypes_, UseDefaultTypes_>::get(const std::string &url, bool loadIfNotFound)
	{
		// Get it from the resource manager
		auto pAsset = asset_warehouse::get().find<T>(url);
		// If not found try to load it through the asset manager
		if( !pAsset && loadIfNotFound )
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
		auto pLoader = findLoader<T>(url);
		if(pLoader)
		{
			filesystem::stream_ptr pStream = filesystem::browser::get().openReadStream(url);
			if(pStream)
			{
				pAsset = pLoader->loadFromStream(*pStream, url);
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
		auto pLoader = findLoader<T>(url);
		if(pLoader)
		{
			filesystem::stream_ptr pStream = filesystem::browser::get().openWriteStream(url);
			if(pStream)
			{
				pLoader->saveToStream(obj, pStream, url);
			}
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename ExtraAssetsTypes_, bool UseDefaultTypes_>
	template<typename T>
	typename loader_holder<T>::loader_sptr
	asset_finder<ExtraAssetsTypes_, UseDefaultTypes_>::findLoader(const std::string &url)
	{
		std::string extension = utils::get_file_extension(url);
		extension = utils::to_lower_case(extension);

		typename loader_holder<T>::loader_sptr pLoader;
		auto it = loader_holder<T>::loaders_.find(extension);
		if( it != loader_holder<T>::loaders_.end() )
			pLoader = it->second;

		return pLoader;
	}
	//----------------------------------------------------------------------------------------------

} /*resources*/ } /*djah*/