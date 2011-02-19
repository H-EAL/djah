namespace djah { namespace filesystem {

	//----------------------------------------------------------------------------------------------
	template<typename CompressionTechnique>
	compressed_source<CompressionTechnique>::compressed_source(const std::string &url, unsigned int priority)
		: source(priority)
		, compressed_stream_(browser::get().openReadStream(url))
	{
		CompressionTechnique::populate_file_registry(compressed_stream_, file_registry_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename CompressionTechnique>
	compressed_source<CompressionTechnique>::~compressed_source()
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename CompressionTechnique>
	stream_ptr compressed_source<CompressionTechnique>::loadStream(const std::string &url)
	{
		stream_ptr strm;
		file_registry_t::const_iterator it = file_registry_.find(url);
		if(it != file_registry_.end())
		{
			const file_struct &fstruct = it->second;
			strm = CompressionTechnique::decompress(compressed_stream_, fstruct);
		}
		return strm;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename CompressionTechnique>
	stream_ptr compressed_source<CompressionTechnique>::saveStream(const std::string &url)
	{
		stream_ptr s;
		return s;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename CompressionTechnique>
	bool compressed_source<CompressionTechnique>::isFetchable(const std::string &url) const
	{
		return file_registry_.find(url) != file_registry_.end();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename CompressionTechnique>
	bool compressed_source<CompressionTechnique>::isWritable(const std::string &url) const
	{
		return false;
	}
	//----------------------------------------------------------------------------------------------

} /*filesystem*/ } /*djah*/