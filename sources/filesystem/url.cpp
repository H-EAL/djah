#include "djah/filesystem/url.hpp"

namespace djah { namespace filesystem {

	//----------------------------------------------------------------------------------------------
	url::url(const std::string &urlStr)
		: urlStr_(urlStr)
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
			+ dirName_
			+ (dirName_.empty() ? "" : "/")
			+ baseName();
	}
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	const std::string url::baseName() const
	{
		return fileName_
			+ (extension_.empty() ? "" : ".")
			+ extension_; 
	}
	//----------------------------------------------------------------------------------------------



	//----------------------------------------------------------------------------------------------
	void url::parse()
	{
		// protocol://branch/.../path/fileName.extension

		const size_t protocolEndsAt = urlStr_.find("://");

		size_t dirNameBeginsAt = 0;
		size_t fileNameBeginsAt = 0;

		if( protocolEndsAt != std::string::npos )
		{
			// Protocol found
			protocol_ = urlStr_.substr(0, protocolEndsAt);
			dirNameBeginsAt = protocolEndsAt+3;
			fileNameBeginsAt = dirNameBeginsAt+1;
		}

		size_t dirNameEndsAt = urlStr_.find_last_of("/\\");
		if( dirNameEndsAt != std::string::npos && dirNameEndsAt > dirNameBeginsAt )
		{
			// Dir name found
			dirName_ = urlStr_.substr(dirNameBeginsAt, dirNameEndsAt - dirNameBeginsAt);
			fileNameBeginsAt = dirNameEndsAt+1;
		}

		const size_t extensionBeginsAt = urlStr_.find_last_of('.');

		fileName_ = urlStr_.substr(fileNameBeginsAt, extensionBeginsAt - fileNameBeginsAt);

		if( extensionBeginsAt != std::string::npos )
		{
			// Extension found
			extension_ = urlStr_.substr(extensionBeginsAt+1);
		}
	}
	//----------------------------------------------------------------------------------------------

} /*filesystem*/ } /*djah*/