#include <string>
#include "fs/memory_stream.hpp"

namespace djah { namespace fs {

	//----------------------------------------------------------------------------------------------
	memory_stream::memory_stream(void *buffer, size_t size)
		: buffer_(new byte[size])
		, buffer_size_(size)
		, r_cursor_(buffer_.get())
	{
		memcpy(buffer_.get(), buffer, size);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	memory_stream::memory_stream(stream_ptr other_stream)
		: buffer_(new byte[other_stream->size()])
		, buffer_size_(other_stream->size())
		, r_cursor_(buffer_.get())
	{
		other_stream->seek(0, ESD_BEG);
		other_stream->read(buffer_.get(), other_stream->size());
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	memory_stream::memory_stream(stream_ptr other_stream, size_t size, size_t offset)
		: buffer_(new byte[size])
		, buffer_size_(size)
		, r_cursor_(buffer_.get())
	{
		other_stream->seek(offset, ESD_BEG);
		other_stream->read(buffer_.get(), size);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	const byte* memory_stream::buffer() const
	{
		return buffer_.get();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	std::string memory_stream::toString() const
	{
		std::string str = reinterpret_cast<const char*>(buffer());
		str.resize(buffer_size_);
		return str;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	size_t memory_stream::size()
	{
		return buffer_size_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool memory_stream::eof()
	{
		return static_cast<size_t>(r_cursor_ - buffer_.get()) > buffer_size_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void memory_stream::seek(size_t offset, E_SEEK_DIR dir)
	{
		switch(dir)
		{
		case ESD_BEG:
			r_cursor_ = buffer() + offset;
			break;
		case ESD_CUR:
			r_cursor_ += offset;
			break;
		case ESD_END:
			r_cursor_ = buffer() + (buffer_size_ - offset);
			break;
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void memory_stream::close()
	{

	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	size_t memory_stream::readImpl(char* buff, size_t size)
	{
		memcpy(buff, buffer(), size);
		return size;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	size_t memory_stream::writeImpl(const char* buff, size_t size)
	{
		memcpy(buffer_.get(), buff, size);
		return size;
	}
	//----------------------------------------------------------------------------------------------

} /*fs*/ } /*djah*/