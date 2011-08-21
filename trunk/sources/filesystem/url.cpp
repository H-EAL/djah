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
			+ branch_path_
			+ (branch_path_.empty() ? "" : "/")
			+ file_name_
			+ (file_extension_.empty() ? "" : ".")
			+ file_extension_; 
	}
	//----------------------------------------------------------------------------------------------



	//----------------------------------------------------------------------------------------------
	void url::parse()
	{
		// protocol://branch/.../path/fileName.extension

		const size_t protocolEndsAt = url_str_.find("://");

		size_t branchPathBeginsAt = 0;
		size_t fileNameBeginsAt = 0;

		if( protocolEndsAt != std::string::npos )
		{
			// Protocol found
			protocol_ = url_str_.substr(0, protocolEndsAt);
			branchPathBeginsAt = protocolEndsAt+3;
			fileNameBeginsAt = branchPathBeginsAt+1;
		}

		size_t branchPathEndsAt = url_str_.find_last_of("/\\");
		if( branchPathEndsAt != std::string::npos && branchPathEndsAt > branchPathBeginsAt )
		{
			// Branch path found
			branch_path_ = url_str_.substr(branchPathBeginsAt, branchPathEndsAt - branchPathBeginsAt);
			fileNameBeginsAt = branchPathEndsAt+1;
		}

		const size_t extensionBeginsAt = url_str_.find_last_of('.');

		file_name_ = url_str_.substr(fileNameBeginsAt, extensionBeginsAt - fileNameBeginsAt);

		if( extensionBeginsAt != std::string::npos )
		{
			// Extension found
			file_extension_ = url_str_.substr(extensionBeginsAt+1);
		}
	}
	//----------------------------------------------------------------------------------------------

} /*filesystem*/ } /*djah*/