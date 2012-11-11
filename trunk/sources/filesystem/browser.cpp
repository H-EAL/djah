#include "djah/filesystem/browser.hpp"
#include "djah/filesystem/source.hpp"

namespace djah { namespace filesystem {

	//----------------------------------------------------------------------------------------------
	browser::~browser()
	{
		source_list_t::iterator it;
		source_list_t::iterator it_end = loading_channels_.end();
		for(it = loading_channels_.begin(); it != it_end; ++it)
			delete *it;

		it_end = saving_channels_.end();
		for(it = saving_channels_.begin(); it != it_end; ++it)
			delete *it;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void browser::addLoadingChannel(source *src)
	{
		loading_channels_.insert(src);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void browser::addSavingChannel(source *src)
	{
		saving_channels_.insert(src);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	stream_ptr browser::openReadStream(const std::string &url)
	{
		stream_ptr strm;

		source_list_t::iterator it;
		source_list_t::iterator it_end = loading_channels_.end();
		for(it = loading_channels_.begin(); it != it_end && !strm; ++it)
		{
			if( (*it)->isFetchable(url) )
			{
				strm = (*it)->loadStream(url);
			}
		}

		return strm;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	stream_ptr browser::openWriteStream(const std::string &url)
	{
		stream_ptr strm;

		source_list_t::iterator it;
		source_list_t::iterator it_end = saving_channels_.end();
		for(it = saving_channels_.begin(); it != it_end && !strm; ++it)
		{
			if( (*it)->isWritable(url) )
			{
				strm = (*it)->saveStream(url);
			}
		}

		return strm;
	}
	//----------------------------------------------------------------------------------------------

} /*filesystem*/ } /*djah*/