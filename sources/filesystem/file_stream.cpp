#include "filesystem/file_stream.hpp"

namespace djah { namespace filesystem {

	//----------------------------------------------------------------------------------------------
	file_stream::file_stream(const std::string &file_name, bool overwrite)
		: file_name_(file_name)
		, overwrite_(overwrite)
	{
		std::ios::openmode mode = std::ios::in | std::ios::binary;

		if(overwrite_)
			mode |= std::ios::trunc | std::ios::out;

		stream_.open(file_name_.c_str(), mode);
		is_open_ = stream_.good();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	file_stream::~file_stream()
	{
		if(is_open_)
			close();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	size_t file_stream::size()
	{
		std::istream::pos_type prevPos = stream_.tellg();
		stream_.seekg(0, std::ios::end);
		std::istream::pos_type length = stream_.tellg();
		stream_.seekg(prevPos, std::ios::beg);

		return static_cast<size_t>(length);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool file_stream::eof()
	{
		return is_open_ ? stream_.eof() : true;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void file_stream::close()
	{
		if(is_open_)
		{
			stream_.flush();
			stream_.close();
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void file_stream::seek(size_t offset, E_SEEK_DIR dir)
	{
		//stream_.seekg(static_cast<std::streamoff>(offset), static_cast<std::ios_base::seekdir>(dir));
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	size_t file_stream::readImpl(char* buff, size_t size)
	{
		size_t count = 0;
		if(is_open_)
		{
			stream_.read(buff, static_cast<std::streamsize>(size));
			count = (size_t)stream_.gcount();
		}
		return count;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	size_t file_stream::writeImpl(const char* buff, size_t size)
	{
		size_t count = 0;
		if (is_open_)
		{
			size_t before = (size_t)stream_.tellp();
			stream_.write(buff, static_cast<std::streamsize>(size));
			count = (size_t)stream_.tellp() - before;
		}
		return count;
	}
	//----------------------------------------------------------------------------------------------

} /*filesystem*/ } /*djah*/