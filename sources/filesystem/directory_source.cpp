#include "djah/filesystem/directory_source.hpp"

//#include <boost/filesystem.hpp>

#include "djah/filesystem/file_stream.hpp"

namespace djah { namespace filesystem {

	//----------------------------------------------------------------------------------------------
	directory_source::directory_source(const std::string &path, bool overwrite, unsigned int priority)
		: source(priority)
		, path_(path)
		, overwrite_(overwrite)
	{

	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	directory_source::~directory_source()
	{

	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	stream_sptr directory_source::newStream(const std::string &url)
	{
		stream_sptr file_strm( new file_stream(path_ + "/" + url, overwrite_) );
		return file_strm;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	stream_sptr directory_source::loadStream(const std::string &url)
	{
		return newStream(url);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	stream_sptr directory_source::saveStream(const std::string &url)
	{
		overwrite_ = true;
		return newStream(url);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool directory_source::isFetchable(const std::string &url) const
	{
		std::fstream strm;
		strm.open((path_ + "/" + url).c_str(), std::ios::in);
		const bool fetchable = strm.good();
		strm.close();
		return fetchable;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool directory_source::isWritable(const std::string &url) const
	{
		/*boost::filesystem::path file(path_);
		file = file / url;
		return exists( file.branch_path() );*/
		return true;
	}
	//----------------------------------------------------------------------------------------------

} /*filesystem*/ } /*djah*/