#ifndef DJAH_FS_FILESYSTEM_HPP
#define DJAH_FS_FILESYSTEM_HPP

#include <set>
#include "../utils/singleton.hpp"
#include "source.hpp"
#include "stream.hpp"

namespace djah { namespace fs {

	class source;

	class filesystem
		: public utils::singleton<filesystem>
	{
		DJAH_MAKE_SINGLETON(filesystem);

	public:

		void addLoadingChannel(source *src);
		void addSavingChannel(source *src);

		stream_ptr openReadStream(const std::string &url);
		stream_ptr openWriteStream(const std::string &url);

	private:

		~filesystem();

		// Predicate used to compare 2 sources
		struct source_comp { bool operator ()(const source *lhs, const source *rhs) { return *lhs < *rhs; } };
		typedef std::multiset<source*, source_comp> source_list_t;

		source_list_t loading_channels_;
		source_list_t saving_channels_;
	};

} /*fs*/ } /*djah*/

#endif /* DJAH_FS_FILESYSTEM_HPP */