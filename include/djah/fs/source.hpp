#ifndef DJAH_FS_SOURCE_HPP
#define DJAH_FS_SOURCE_HPP

#include <string>
#include <boost/shared_ptr.hpp>
#include "stream.hpp"

namespace djah { namespace fs {

	class source
	{
	public:

		source(unsigned int priority) : priority_(priority) {}
		virtual ~source() {}

		virtual stream_ptr loadStream(const std::string &url) = 0;
		virtual stream_ptr saveStream(const std::string &url) = 0;

		virtual bool isFetchable(const std::string &url) const = 0;
		virtual bool isWritable(const std::string &url)  const = 0;

		friend bool operator <(const source &lhs, const source &rhs)
		{ return lhs.priority_ < rhs.priority_; }

	private:

		unsigned int priority_;
	};

} /*fs*/ } /*djah*/

#endif /* DJAH_FS_SOURCE_HPP */