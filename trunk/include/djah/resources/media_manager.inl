namespace djah { namespace resources {

	//----------------------------------------------------------------------------------------------
	template<typename ExtraTypes>
	template<typename T>
	void media_manager<ExtraTypes>::registerLoader(loader<T> *loader, const std::string &ext)
	{
		std::vector<std::string> extensions;
		utils::split_string(ext, extensions, " /\\*.,;|-_\t\n'\"");

		loader_t_ptr p_loader(loader);
		std::vector<std::string>::const_iterator it;
		std::vector<std::string>::const_iterator it_end = extensions.end();
		for(it = extensions.begin(); it != it_end; ++it)
		{
			std::string extension = utils::to_lower_case(*it);
			media_holder<T>::loaders_[extension] = p_loader;
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename ExtraTypes>
	template<typename T>
	T* media_manager<ExtraTypes>::loadFromUrl(const std::string &url)
	{
		T *res = 0;
		media_holder<T>::loader_t_ptr ldr = findLoader<T>(url);
		if(ldr)
		{
			fs::stream_ptr strm = fs::filesystem::get_instance().openReadStream(url);
			if(strm)
				res = ldr->loadFromStream(*strm, url);
		}
		return res;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename ExtraTypes>
	template<typename T>
	void media_manager<ExtraTypes>::saveToUrl(const T &obj, const std::string &url)
	{
		media_holder<T>::loader_t_ptr ldr = findLoader<T>(url);
		if(ldr)
		{
			fs::stream_ptr strm = fs::filesystem::get_instance().openWriteStream(url);
			if(strm)
				res = ldr->saveToStream(obj, strm, url);
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename ExtraTypes>
	template<typename T>
	typename media_holder<T>::loader_t_ptr media_manager<ExtraTypes>::findLoader(const std::string &url)
	{
		std::string extension = utils::get_file_extension(url);
		extension = utils::to_lower_case(extension);

		media_holder<T>::loader_t_ptr ldr;
		media_holder<T>::loaders_map_t::const_iterator it = media_holder<T>::loaders_.find(extension);
		if( it != media_holder<T>::loaders_.end() )
			ldr = it->second;

		return ldr;
	}
	//----------------------------------------------------------------------------------------------

} /*resources*/ } /*djah*/