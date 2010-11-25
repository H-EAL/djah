#include "fs/filesystem.hpp"
#include "fs/source.hpp"

namespace djah { namespace fs {

	//----------------------------------------------------------------------------------------------
	filesystem::~filesystem()
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
	void filesystem::addLoadingChannel(source *src)
	{
		loading_channels_.insert(src);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void filesystem::addSavingChannel(source *src)
	{
		saving_channels_.insert(src);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	stream_ptr filesystem::openReadStream(const std::string &url)
	{
		stream_ptr strm;

		source_list_t::iterator it;
		source_list_t::iterator it_end = loading_channels_.end();
		for(it = loading_channels_.begin(); it != it_end; ++it)
		{
			if( (*it)->isFetchable(url) )
			{
				strm = (*it)->loadStream(url);
				break;
			}
		}

		return strm;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	stream_ptr filesystem::openWriteStream(const std::string &url)
	{
		stream_ptr strm;

		source_list_t::iterator it;
		source_list_t::iterator it_end = saving_channels_.end();
		for(it = saving_channels_.begin(); it != it_end; ++it)
		{
			if( (*it)->isWritable(url) )
			{
				strm = (*it)->saveStream(url);
				break;
			}
		}

		return strm;
	}
	//----------------------------------------------------------------------------------------------

} /*fs*/ } /*djah*/