#ifndef DJAH_FS_MEMORY_STREAM_HPP
#define DJAH_FS_MEMORY_STREAM_HPP

#include <boost/scoped_array.hpp>
#include "../types.hpp"
#include "stream.hpp"

namespace djah { namespace fs {

	class memory_stream
		: public stream
	{
	public:
		memory_stream(void *buffer, u32 size);

		size_t size();
		bool eof();
		void close();

	private:
		size_t readImpl(byte* buff, size_t size);
		size_t writeImpl(const byte* buff, size_t size);

		boost::scoped_array<byte>	buffer_;
		u32							buffer_size_;
		byte						*cursor_;
	};

} /*fs*/ } /*djah*/

#endif /* DJAH_FS_MEMORY_STREAM_HPP */