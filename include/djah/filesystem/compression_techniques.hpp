#ifndef DJAH_FILESYSTEM_COMPRESSION_TECHNIQUES_HPP
#define DJAH_FILESYSTEM_COMPRESSION_TECHNIQUES_HPP

#include "stream.hpp"
#include "compressed_source.hpp"

namespace djah { namespace filesystem {

	#define COMPRESSION_TECHNIQUE_FUNCTIONS \
	static void populate_file_registry(stream_ptr &pStream, file_registry_t &registry); \
	static stream_ptr decompress(stream_ptr &compressed_stream, const file_struct &fstruct);

	struct pak_compression
	{
		COMPRESSION_TECHNIQUE_FUNCTIONS
	};

	struct zip_compression
	{
		COMPRESSION_TECHNIQUE_FUNCTIONS
	};

	#undef COMPRESSION_TECHNIQUE_FUNCTIONS

	typedef compressed_source<pak_compression> pak_source;
	typedef compressed_source<zip_compression> zip_source;

} /*filesystem*/ } /*djah*/

#endif /* DJAH_FILESYSTEM_COMPRESSION_TECHNIQUES_HPP */