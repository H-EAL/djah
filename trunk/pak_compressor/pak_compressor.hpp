#ifndef PAK_COMPRESSOR_HPP
#define PAK_COMPRESSOR_HPP

#include <set>
#include <string>
#include <boost/filesystem.hpp>
#include <djah/types.hpp>
#include <djah/fs/stream.hpp>

class pak_compressor
{
public:

	static void init();
	static int  show_help();
	static int	run(int argc, char *argv[]);

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

	bool addFile(const boost::filesystem::path &file);
	void writeHeaders();
	void writeFiles();
	void writeCRC();



	file_list_t				files_;
	std::string				dir_name_;
	std::string				pak_name_;
	djah::fs::stream_ptr	pak_file_;
	djah::u32				crc_;
};

#endif /* PAK_COMPRESSOR_HPP */