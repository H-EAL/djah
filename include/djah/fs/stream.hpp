#ifndef DJAH_FS_STREAM_HPP
#define DJAH_FS_STREAM_HPP

#include <boost/shared_ptr.hpp>

namespace djah { namespace fs {

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

			template<typename T>
			stream& operator >>(T &toRead);
			template<typename T>
			stream& operator <<(const T &toWrite);

			virtual size_t size() = 0;
			virtual bool eof() = 0;
			virtual void close() = 0;

	protected:

		typedef char byte;

		virtual size_t readImpl(byte* buff, size_t size) = 0;
		virtual size_t writeImpl(const byte* buff, size_t size) = 0;
	};

	typedef boost::shared_ptr<stream> stream_ptr;

} /*fs*/ } /*djah*/

#include "stream.inl"

#endif /* DJAH_FS_STREAM_HPP */