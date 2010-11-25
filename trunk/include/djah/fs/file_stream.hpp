#ifndef DJAH_FS_FILE_STREAM_HPP
#define DJAH_FS_FILE_STREAM_HPP

#include <fstream>
#include "stream.hpp"

namespace djah { namespace fs {

	class file_stream
		: public stream
	{
	public:

		file_stream(const std::string &file_name, bool overwrite = false);
		~file_stream();

		size_t size();
		bool   eof();
		void   close();

	private:

		size_t readImpl(byte* buff, size_t size);
		size_t writeImpl(const byte* buff, size_t size);

		std::string		file_name_;
		bool			overwrite_;
		std::fstream	stream_;
		bool			is_open_;
	};

} /*fs*/ } /*djah*/

#endif /* DJAH_FS_FILE_STREAM_HPP */