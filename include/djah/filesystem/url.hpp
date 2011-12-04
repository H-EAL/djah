#ifndef DJAH_FILESYSTEM_URL_HPP
#define DJAH_FILESYSTEM_URL_HPP

#include <string>

namespace djah { namespace filesystem {

	class url
	{
	public:

		url(const std::string &urlStr);
		~url();

		const std::string& str()		const { return url_str_;	}
		const std::string& protocol()	const { return protocol_;	}
		const std::string& dirName()	const { return dir_name_;	}
		const std::string& fileName()	const { return file_name_;	}
		const std::string& extension()	const { return extension_;	}
		const std::string  baseName()	const;

		const std::string toString() const;
		url& operator =(const std::string &urlStr) { *this = url(urlStr); return (*this); }
		
	private:

		void parse();

	private:

		std::string url_str_;
		std::string protocol_;
		std::string dir_name_;
		std::string file_name_;
		std::string extension_;
	};

	const url operator /(const url &lhs, const url &rhs);

} /*filesystem*/ } /*djah*/

#endif /* DJAH_FILESYSTEM_URL_HPP */