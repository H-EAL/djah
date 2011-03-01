#include "filesystem/compression_techniques.hpp"
#include "filesystem/memory_stream.hpp"

namespace djah { namespace filesystem {
	
	//----------------------------------------------------------------------------------------------
	namespace {
		
		struct file_entry
		{
			// Max file name size
			enum { FILENAME_MAX_SIZE = 32 };
			
			char file_name_[FILENAME_MAX_SIZE];
			u32	 size_;
			u32	 offset_;
		} fentry;
	}
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	void pak_compression::populate_file_registry(stream_ptr &strm, file_registry_t &registry)
	{
		// Make sure we're at the beginning of the file
		strm->seek(0, ESD_BEG);

		// Read the file number
		u32 num_files_ = 0;
		strm->read(num_files_);

		// Read as many headers as there are files
		for(u32 i = 0; i < num_files_; ++i)
		{
			file_entry fentry;
			strm->read(fentry);
			file_struct fstruct = { fentry.offset_, fentry.size_, fentry.size_ };
			registry.insert( file_registry_t::value_type(fentry.file_name_, fstruct) );
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	stream_ptr pak_compression::decompress(stream_ptr &strm, const file_struct &fstruct)
	{
		return stream_ptr( new memory_stream(strm, fstruct.real_size_, fstruct.offset_) );
	}
	//----------------------------------------------------------------------------------------------

} /*filesystem*/ } /*djah*/