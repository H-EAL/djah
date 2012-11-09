#ifndef DJAH_DEBUG_MEMORY_TRACKER_HPP
#define DJAH_DEBUG_MEMORY_TRACKER_HPP

#include <map>
#include <stack>
#include "../types.hpp"

namespace djah { namespace debug {

	class memory_tracker
	{
	public:
		struct block_info_t
		{
			void		*pAddress;
			size_t		size;
			const char	*file;
			int			line;
			bool		isArray;
		};

	public:
		enum
		{
			ALLOCATED_GARBAGE   = 0xCC,
			DEALLOCATED_GARBAGE = 0xFF
		};

	public:
		static memory_tracker& get();

		bool allocateBlock(void **ppv, size_t size, const char *file, int line, bool isArray);
		void deallocateBlock(void *pv, bool isArray);
		void nextDelete(const char *file, int line);

	private:
		memory_tracker();
		~memory_tracker();
		void registerBlock(void *pv, size_t size, const char *file, int line, bool isArray);
		void unregisterBlock(void *pv, bool isArray);
		size_t sizeOfBlock(void *pv);

	private:
		typedef std::map<void*, block_info_t>	block_list_t;
		typedef std::stack<block_info_t>		block_stack_t;
		block_list_t	blocks_;
		block_stack_t	blocksToDelete_;
	};

} /*debug*/ } /*djah*/

#endif /* DJAH_DEBUG_MEMORY_TRACKER_HPP */