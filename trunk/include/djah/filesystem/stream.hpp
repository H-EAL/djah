#ifndef DJAH_FILESYSTEM_STREAM_HPP
#define DJAH_FILESYSTEM_STREAM_HPP

#include <ios>
#include <string>
#include <boost/shared_ptr.hpp>
#include "../types.hpp"

namespace djah { namespace filesystem {

	enum E_SEEK_DIR
	{
		ESD_BEG = std::ios::beg,
		ESD_CUR = std::ios::cur,
		ESD_END = std::ios::end
	};

	class stream
	{
	public:

		stream() {};
		virtual ~stream() {};

		template<typename T>
		size_t read(T &data);
		template<typename T, size_t N>
		size_t read(T (&data_array)[N]);
		template<typename T>
		size_t read(T *data_ptr, size_t count);

		template<typename T>
		size_t write(const T &data);
		template<typename T, size_t N>
		size_t write(const T (&data_array)[N]);
		template<typename T>
		size_t write(const T *data_ptr, size_t count);
		template<typename Char_T>
		size_t write(const std::basic_string<Char_T> &str);

		template<typename T>
		stream& operator >>(T &toRead);
		template<typename T>
		stream& operator <<(const T &toWrite);

		virtual size_t size() = 0;
		virtual bool   eof() = 0;
		virtual void   seek(size_t offset, E_SEEK_DIR dir) = 0;
		virtual void   close() = 0;

	protected:

		virtual size_t readImpl(char* buff, size_t size) = 0;
		virtual size_t writeImpl(const char* buff, size_t size) = 0;
	};

	typedef boost::shared_ptr<stream> stream_ptr;

} /*filesystem*/ } /*djah*/

#include "stream.inl"

#endif /* DJAH_FILESYSTEM_STREAM_HPP */