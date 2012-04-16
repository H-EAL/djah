#ifndef DJAH_FILESYSTEM_MEMORY_STREAM_HPP
#define DJAH_FILESYSTEM_MEMORY_STREAM_HPP

#include "../types.hpp"
#include "stream.hpp"

namespace djah { namespace filesystem {

	class memory_stream
		: public stream
	{
	public:
		memory_stream(void *buffer, size_t size);
		memory_stream(stream_ptr other_stream);
		memory_stream(stream_ptr other_stream, size_t size, size_t offset = 0);
		virtual ~memory_stream();

		const byte* buffer() const;
		std::string toString() const;

		size_t	size();
		bool	eof();
		void	seek(size_t offset, E_SEEK_DIR dir);
		void	close();

	private:
		size_t readImpl(char* buff, size_t size);
		size_t writeImpl(const char* buff, size_t size);

		byte						*buffer_;
		size_t						buffer_size_;
		const byte					*r_cursor_;
		byte						*w_cursor_;
	};

} /*filesystem*/ } /*djah*/

#endif /* DJAH_FILESYSTEM_MEMORY_STREAM_HPP */