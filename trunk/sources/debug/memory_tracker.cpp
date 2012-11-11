#include "djah/debug/memory_tracker.hpp"
#include "djah/debug/assertion.hpp"

#include <cstdlib>


namespace djah { namespace debug {

	//----------------------------------------------------------------------------------------------
	memory_tracker& memory_tracker::get()
	{
		static memory_tracker sInstance;
		return sInstance;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	memory_tracker::memory_tracker()
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	memory_tracker::~memory_tracker()
	{
		DJAH_ASSERT_MSG( blocks_.empty(), "Memory leak" );
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool memory_tracker::allocateBlock(void **ppv, size_t size, const char *file, int line, bool isArray)
	{
		DJAH_ASSERT_MSG( ppv != nullptr, "Invalid pointer" );
		DJAH_ASSERT_MSG( size != 0, "Invalid memory block size" );

		byte **ppb = reinterpret_cast<byte**>(ppv);
		*ppb = reinterpret_cast<byte*>( malloc(size) );
		registerBlock(*ppv, size, file, line, isArray);

#ifdef MEMORY_DEBUG
		if( ppb != nullptr )
		{
			memset(*ppb, ALLOCATED_GARBAGE, size);
		}
#endif

		return (*ppb != nullptr);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void memory_tracker::deallocateBlock(void *pv, bool isArray)
	{
		DJAH_ASSERT_MSG( pv != nullptr, "Invalid pointer" );

#ifdef MEMORY_DEBUG
		memset(pv, DEALLOCATED_GARBAGE, sizeOfBlock(pv));
#endif

		unregisterBlock(pv, isArray);
		free(pv);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void memory_tracker::nextDelete(const char *file, int line)
	{
		block_info_t block;
		block.file = file;
		block.line = line;

		blocksToDelete_.push(block);
	}
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	void memory_tracker::registerBlock(void *pv, size_t size, const char *file, int line, bool isArray)
	{
		DJAH_ASSERT_MSG( pv != nullptr, "Invalid pointer" );

		block_list_t::const_iterator itBlock = blocks_.find(pv);

		DJAH_ASSERT_MSG( itBlock == blocks_.end(), "Trying to allocate a non available memory space" );

		const block_info_t bi = { pv, size, file, line, isArray };
		blocks_.insert( block_list_t::value_type(pv, bi) );
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	void memory_tracker::unregisterBlock(void *pv, bool isArray)
	{
		block_list_t::const_iterator itBlock = blocks_.find(pv);

		DJAH_ASSERT_MSG( itBlock != blocks_.end(), "Trying to free an invalid memory block" );
		DJAH_ASSERT_MSG( itBlock->second.isArray == isArray, "Bad delete operator used" );

		blocks_.erase( itBlock );
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	size_t memory_tracker::sizeOfBlock(void *pv)
	{
		block_list_t::const_iterator itBlock = blocks_.find(pv);
		DJAH_ASSERT_MSG( itBlock != blocks_.end(), "Unable to find block size" );

		return itBlock->second.size;
	}
	//----------------------------------------------------------------------------------------------

} /*debug*/ } /*djah*/