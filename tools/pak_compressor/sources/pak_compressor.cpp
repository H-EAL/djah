#include <iostream>
#include <memory>

#include <djah/debug/log.hpp>
#include <djah/debug/console_sink.hpp>
#include <djah/filesystem/browser.hpp>
#include <djah/filesystem/directory_source.hpp>
#include <djah/filesystem/memory_stream.hpp>
#include <djah/core/time/timer.hpp>

#include "pak_compressor.hpp"

using namespace boost::filesystem;

//--------------------------------------------------------------------------------------------------
void pak_compressor::init()
{
	djah::debug::core_logger::get().addSink( std::make_shared<djah::debug::console_sink>() );
	djah::debug::core_logger::get().addSink( std::make_shared<djah::debug::output_debug_sink>() );
	djah::filesystem::browser::get().addLoadingChannel(new djah::filesystem::directory_source("."));
	djah::filesystem::browser::get().addSavingChannel(new djah::filesystem::directory_source(".", true));
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
int pak_compressor::show_help()
{
	DJAH_GLOBAL_CRITICAL()
		<< "Usage: pak_compressor folder_to_pak [pak_file_name]"
		<< DJAH_END_LOG();

	getchar();

	return EXIT_FAILURE;
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
int pak_compressor::run(int argc, char *argv[])
{
	init();
	return (argc < 2)	? pak_compressor::show_help()
						: pak_compressor(argv[1], argc > 3 ? argv[2] : "").compress();

}
//--------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------
pak_compressor::pak_compressor(const std::string &dir_name, const std::string &pak_name)
	: dir_name_(dir_name)
	, pak_name_(pak_name.empty() ? dir_name : pak_name)
	, crc_(0)
{
	pak_name_ += ".pak";
	pak_file_ = djah::filesystem::browser::get().openWriteStream(pak_name_);
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
int pak_compressor::compress()
{
	path dir_path(dir_name_);
	if(!pak_file_ || !exists(dir_path))
		return EXIT_FAILURE;

	boost::filesystem::directory_iterator it_end;
	for(directory_iterator it(dir_path); it != it_end; ++it)
	{
		if(boost::filesystem::is_regular_file(it->status()))
		{
			if( !addFile(it->path()) )
			{
				DJAH_GLOBAL_WARNING() << "Skipping: " << it->path().filename() << DJAH_END_LOG();
			}
		}
	}

	if(!files_.empty())
	{
		writeHeaders();
		writeFiles();
	}

	getchar();

	return EXIT_SUCCESS;
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
bool pak_compressor::addFile(const boost::filesystem::path &file)
{
	if(file.filename().size() >= FILENAME_MAX_SIZE)
		return false;

	pak_header header;
	memset(header.file_name_, 0, FILENAME_MAX_SIZE);
	memcpy(header.file_name_, file.filename().c_str(), file.filename().size());

	header.size_	= static_cast<djah::u32>(boost::filesystem::file_size(file));
	header.offset_	= 0;
	
	files_.push_back(header);
	return true;
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void pak_compressor::writeHeaders()
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
	DJAH_GLOBAL_NOTIFICATION() << "--------------------------------------------------" << DJAH_END_LOG();

	file_list_t::iterator it;
	file_list_t::iterator it_end = files_.end();
	for(it = files_.begin(); it != it_end; ++it)
	{
		djah::filesystem::stream_ptr file = djah::filesystem::browser::get().openReadStream(dir_name_ + "/" + it->file_name_);
		if(file)
		{
			DJAH_GLOBAL_NOTIFICATION() << "Packing " << it->file_name_ << " ... ";

			djah::time::timer clk;
			djah::filesystem::memory_stream buff(file);
			pak_file_->write(buff.buffer(), buff.size());
			djah::f32 dt = clk.getElapsedTimeMs();

			DJAH_GLOBAL_NOTIFICATION()	<< "done (" << dt << " ms" ")" << DJAH_END_LOG();
		}
	}
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void pak_compressor::writeCRC()
{
	pak_file_->write(crc_);
}
//--------------------------------------------------------------------------------------------------