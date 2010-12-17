#ifndef PAK_COMPRESSOR_HPP
#define PAK_COMPRESSOR_HPP

#include <set>
#include <string>
#include <djah/types.hpp>
#include <boost/filesystem.hpp>

class pak_compressor
{
public:

	static int show_help();

	pak_compressor(const std::string &dir_name, const std::string &pak_name = "");

	int compress();

private:

	static const int FILENAME_MAX_SIZE = 32;
	struct pak_header
	{
		char		file_name_[FILENAME_MAX_SIZE];
		djah::u32	size_;
		djah::u32	offset_;

		bool operator <(const pak_header &rhs) const
		{ return strncmp(file_name_, rhs.file_name_, FILENAME_MAX_SIZE) < 0; }
	};

	typedef std::set<pak_header> file_list_t;

	bool fillPakHeader(const boost::filesystem::path &file);
	void writeHeader();
	void writeFiles();



	file_list_t				files_;
	std::string				dir_name_;
	std::string				pak_name_;
	djah::fs::stream_ptr	pak_file_;
};

#endif /* PAK_COMPRESSOR_HPP */