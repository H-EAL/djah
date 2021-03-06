#ifndef DJAH_FILESYSTEM_DIRECTORY_SOURCE_HPP
#define DJAH_FILESYSTEM_DIRECTORY_SOURCE_HPP

#include "source.hpp"

namespace djah { namespace filesystem {

	class directory_source
		: public source
	{
	public:

		directory_source(const std::string &path, bool overwrite = false, unsigned int priority = 0);
		virtual ~directory_source();

		virtual stream_sptr loadStream(const std::string &url);
		virtual stream_sptr saveStream(const std::string &url);

		virtual bool isFetchable(const std::string &url) const;
		virtual bool isWritable (const std::string &url) const;

	private:

		stream_sptr newStream(const std::string &url);

		std::string path_;
		bool		overwrite_;
	};

} /*filesystem*/ } /*djah*/

#endif /* DJAH_FILESYSTEM_DIRECTORY_SOURCE_HPP */