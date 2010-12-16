#include <fs/compressed_source.hpp>

namespace djah { namespace fs {

	//----------------------------------------------------------------------------------------------
	template<typename CompressionTechnique>
	compressed_source<CompressionTechnique>::compressed_source(const std::string &url, unsigned int priority)
		: source(priority)
		, compressed_stream_(filesystem::get_instance().openReadStream(url))
	{
		CompressionTechnique::populate_file_registry();
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
			const file_struct &fstruct = *it;
			boost::scoped_array<byte> buffer( new byte[fstruct.size_] );
			compressed_stream_->seek(fstruct.offset_);
			compressed_stream_->read(buffer.get(), fstruct.size_);
			// Decompress here
			strm = stream_ptr(new memory_stream(buffer.get(), fstruct.size_));
		}
		return strm;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename CompressionTechnique>
	stream_ptr compressed_source<CompressionTechnique>::saveStream(const std::string &url)
	{
		return 0;
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

} /*fs*/ } /*djah*/