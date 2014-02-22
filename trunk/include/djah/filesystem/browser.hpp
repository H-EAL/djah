#ifndef DJAH_FILESYSTEM_BROWSER_HPP
#define DJAH_FILESYSTEM_BROWSER_HPP

#include <set>
#include "djah/core/singleton.hpp"
#include "djah/filesystem/source.hpp"
#include "djah/filesystem/stream.hpp"

namespace djah { namespace filesystem {

	class browser
		: public utils::singleton<browser>
	{
		DJAH_MAKE_SINGLETON(browser);

	public:
		void addLoadingChannel(source *src);
		void addSavingChannel(source *src);

		stream_sptr openReadStream(const std::string &url);
		stream_sptr openWriteStream(const std::string &url);

	private:
		browser();
		virtual ~browser();

	private:
		// Predicate used to compare 2 sources
		struct source_comp { bool operator ()(const source *lhs, const source *rhs) { return *lhs < *rhs; } };
		typedef std::multiset<source*, source_comp> source_list_t;

		source_list_t loading_channels_;
		source_list_t saving_channels_;
	};

} /*filesystem*/ } /*djah*/

#endif /* DJAH_FILESYSTEM_BROWSER_HPP */