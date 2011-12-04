#include "filesystem/url.hpp"

namespace djah { namespace filesystem {

	//----------------------------------------------------------------------------------------------
	url::url(const std::string &urlStr)
		: url_str_(urlStr)
	{
		parse();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	url::~url()
	{
	}
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	const std::string url::toString() const
	{
		return protocol_
			+ (protocol_.empty() ? "" : "://")
			+ dir_name_
			+ (dir_name_.empty() ? "" : "/")
			+ baseName();
	}
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	const std::string url::baseName() const
	{
		return file_name_
			+ (extension_.empty() ? "" : ".")
			+ extension_; 
	}
	//----------------------------------------------------------------------------------------------



	//----------------------------------------------------------------------------------------------
	void url::parse()
	{
		// protocol://branch/.../path/fileName.extension

		const size_t protocolEndsAt = url_str_.find("://");

		size_t dirNameBeginsAt = 0;
		size_t fileNameBeginsAt = 0;

		if( protocolEndsAt != std::string::npos )
		{
			// Protocol found
			protocol_ = url_str_.substr(0, protocolEndsAt);
			dirNameBeginsAt = protocolEndsAt+3;
			fileNameBeginsAt = dirNameBeginsAt+1;
		}

		size_t dirNameEndsAt = url_str_.find_last_of("/\\");
		if( dirNameEndsAt != std::string::npos && dirNameEndsAt > dirNameBeginsAt )
		{
			// Dir name found
			dir_name_ = url_str_.substr(dirNameBeginsAt, dirNameEndsAt - dirNameBeginsAt);
			fileNameBeginsAt = dirNameEndsAt+1;
		}

		const size_t extensionBeginsAt = url_str_.find_last_of('.');

		file_name_ = url_str_.substr(fileNameBeginsAt, extensionBeginsAt - fileNameBeginsAt);

		if( extensionBeginsAt != std::string::npos )
		{
			// Extension found
			extension_ = url_str_.substr(extensionBeginsAt+1);
		}
	}
	//----------------------------------------------------------------------------------------------

} /*filesystem*/ } /*djah*/