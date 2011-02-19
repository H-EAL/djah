#ifndef DJAH_FILESYSTEM_DIRECTORY_SOURCE_HPP
#define DJAH_FILESYSTEM_DIRECTORY_SOURCE_HPP

#include "source.hpp"

namespace djah { namespace filesystem {

	class directory_source
		: public source
	{
	public:

		directory_source(const std::string &path, bool overwrite = false, unsigned int priority = 0);
		~directory_source();

		stream_ptr loadStream(const std::string &url);
		stream_ptr saveStream(const std::string &url);

		bool isFetchable(const std::string &url) const;
		bool isWritable (const std::string &url) const;

	private:

		stream_ptr newStream(const std::string &url);

		std::string path_;
		bool		overwrite_;
	};

} /*filesystem*/ } /*djah*/

#endif /* DJAH_FILESYSTEM_DIRECTORY_SOURCE_HPP */