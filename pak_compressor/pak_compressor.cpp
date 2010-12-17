#include <iostream>
#include <djah/fs/filesystem.hpp>
#include <djah/fs/memory_stream.hpp>
#include "pak_compressor.hpp"

using namespace boost::filesystem;

//--------------------------------------------------------------------------------------------------
int pak_compressor::show_help()
{
	std::cerr << "Usage: pak_compressor folder_to_pak [pak_file_name]" << std::endl;
	getchar();
	return EXIT_FAILURE;
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
pak_compressor::pak_compressor(const std::string &dir_name, const std::string &pak_name)
	: dir_name_(dir_name)
	, pak_name_(pak_name.empty() ? dir_name : pak_name)
{
	pak_name_ += ".pak";
	pak_file_ = djah::fs::stream_ptr(djah::fs::filesystem::get_instance().openWriteStream(pak_name_));
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
int pak_compressor::compress()
{
	path dir_path(dir_name_);
	if(!pak_file_ || !exists(dir_path))
		return EXIT_FAILURE;

	directory_iterator it_end;
	for(directory_iterator it(dir_path); it != it_end; ++it)
	{
		if(boost::filesystem::is_regular_file(it->status()))
		{
			if( fillPakHeader(it->path()) ) std::cout << "Adding: ";
			else							std::cout << "Skiping: ";
			std::cout << it->path().filename() << std::endl;
		}
	}

	if(!files_.empty())
	{
		writeHeader();
		writeFiles();
	}

	return EXIT_SUCCESS;
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
bool pak_compressor::fillPakHeader(const boost::filesystem::path &file)
{
	if(file.filename().size() >= FILENAME_MAX_SIZE)
		return false;

	pak_header header;
	memset(header.file_name_, 0, FILENAME_MAX_SIZE);
	memcpy(header.file_name_, file.filename().c_str(), FILENAME_MAX_SIZE-1);

	header.size_	= static_cast<djah::u32>(file_size(file));
	header.offset_	= 0;

	return files_.insert(header).second;
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void pak_compressor::writeHeader()
{
	djah::u32 file_count = static_cast<djah::u32>(files_.size());
	pak_file_->write(file_count);

	djah::u32 offset = sizeof(djah::u32);
	offset += sizeof(pak_header) * file_count;

	file_list_t::iterator it;
	file_list_t::iterator it_end = files_.end();
	for(it = files_.begin(); it != it_end; ++it)
	{
		it->offset_ = offset;
		pak_file_->write(*it);
		offset += it->size_;
	}
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void pak_compressor::writeFiles()
{
	file_list_t::iterator it;
	file_list_t::iterator it_end = files_.end();
	for(it = files_.begin(); it != it_end; ++it)
	{
		djah::fs::stream_ptr file = djah::fs::filesystem::get_instance().openReadStream(dir_name_ + "/" + it->file_name_);
		if(file)
		{
			djah::fs::memory_stream buff(file);
			pak_file_->write(buff.buffer(), buff.size());
		}
	}
}
//--------------------------------------------------------------------------------------------------