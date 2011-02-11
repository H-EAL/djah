#ifndef DJAH_FS_COMPRESSED_SOURCE_HPP
#define DJAH_FS_COMPRESSED_SOURCE_HPP

#include <map>
#include "source.hpp"
#include "stream.hpp"
#include "compressed_source.hpp"
#include "memory_stream.hpp"
#include "filesystem.hpp"

namespace djah { namespace fs {

	struct file_struct {
		size_t offset_;
		size_t real_size_;
		size_t compressed_size_;
	};

	typedef std::map<std::string, file_struct> file_registry_t;

	template<typename CompressionTechnique>
	class compressed_source
		: public source
	{ 
	public:

		compressed_source(const std::string &url, unsigned int priority = 0);
		~compressed_source();

		stream_ptr loadStream(const std::string &url);
		stream_ptr saveStream(const std::string &url);

		bool isFetchable(const std::string &url) const;
		bool isWritable(const std::string &url)  const;

	private:

		file_registry_t file_registry_;
		stream_ptr		compressed_stream_;
	};

} /*fs*/ } /*djah*/

#include "compressed_source.inl"

#endif /* DJAH_FS_COMPRESSED_SOURCE_HPP */