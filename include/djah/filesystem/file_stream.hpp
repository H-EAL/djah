#ifndef DJAH_FILESYSTEM_FILE_STREAM_HPP
#define DJAH_FILESYSTEM_FILE_STREAM_HPP

#include <fstream>
#include "stream.hpp"

namespace djah { namespace filesystem {

	class file_stream
		: public stream
	{
	public:

		file_stream(const std::string &file_name, bool overwrite = false);
		virtual ~file_stream();

		virtual size_t size();
		virtual bool   eof();
		virtual void   seek(size_t offset, E_SEEK_DIR dir);
		virtual void   close();

	private:

		size_t readImpl(char* buff, size_t size);
		size_t writeImpl(const char* buff, size_t size);

		std::string		file_name_;
		bool			overwrite_;
		std::fstream	stream_;
		bool			is_open_;
		std::size_t		file_size_;
	};

} /*filesystem*/ } /*djah*/

#endif /* DJAH_FILESYSTEM_FILE_STREAM_HPP */