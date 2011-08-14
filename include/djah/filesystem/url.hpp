#ifndef DJAH_FILESYSTEM_URI_HPP
#define DJAH_FILESYSTEM_URI_HPP

#include <string>

namespace djah { namespace filesystem {

	class uri
	{
	public:

		uri(const std::string &uriStr);
		~uri();

		const std::string& str()			const { return uri_str_;			}
		const std::string& completeStr()	const { return complete_uri_str_;	}
		const std::string& protocol()		const { return protocol_;			}
		const std::string& branchPath()		const { return branch_path_;		}
		const std::string& fileName()		const { return file_name_;			}
		const std::string& fileExtension()	const { return file_extension_;		}

		const uri operator =(const uri &rhs);

	private:

		std::string uri_str_;
		std::string complete_uri_str_;
		std::string protocol_;
		std::string branch_path_;
		std::string file_name_;
		std::string file_extension_;
	};

} /*filesystem*/ } /*djah*/

#endif /* DJAH_FILESYSTEM_URI_HPP */