#ifndef DJAH_OPENGL_BUFFERS_STORAGE_POLICIES_HPP
#define DJAH_OPENGL_BUFFERS_STORAGE_POLICIES_HPP

#include "djah/types.hpp"
#include "djah/system/gl.hpp"
#include "djah/debug/assertion.hpp"

namespace djah { namespace opengl {

	//////////////////////////////////////////////////////////////////////////
	// Description:
	//		Allocates
	//////////////////////////////////////////////////////////////////////////
	// Core since version:
	//		3.0
	//////////////////////////////////////////////////////////////////////////
	// Extensions:
	//		- ARB_map_buffer_range
	//////////////////////////////////////////////////////////////////////////
	class Mutable_StoragePolicy
	{
	protected:
		Mutable_StoragePolicy()
			: pBufferPtr_(nullptr)
		{

		}

		void allocateStorage(GLenum _target, u32 _size, void *_pData, GLenum _usage)
		{
			glBufferData(_target, _size, _pData, _usage);
		}

		void invalidate(GLenum _target, u32 _size, GLenum _usage)
		{
			glBufferData(_target, _size, nullptr, _usage);
		}

		void* map(GLenum _target, GLbitfield _access)
		{
			check(pBufferPtr_ == nullptr);
			pBufferPtr_ = glMapBuffer(_target, _access);
			return pBufferPtr_;
		}

		void* mapRange(GLenum _target, u32 _offset, u32 _length, GLbitfield _access)
		{
			check(pBufferPtr_ == nullptr);
			pBufferPtr_ = glMapBufferRange(_target, _offset, _length, _access);
			return pBufferPtr_;
		}

		void unmap(GLenum _target)
		{
			check(pBufferPtr_ != nullptr);
			glUnmapBuffer(_target);
			pBufferPtr_ = nullptr;
		}

	private:
		void *pBufferPtr_;
	};

	//////////////////////////////////////////////////////////////////////////
	// Description:
	//		Allocates
	//////////////////////////////////////////////////////////////////////////
	// Core since version:
	//		4.4
	//////////////////////////////////////////////////////////////////////////
	// Extensions:
	//		- ARB_buffer_storage
	//////////////////////////////////////////////////////////////////////////
	class Immutable_StoragePolicy
	{
	protected:
		void allocateStorage(GLenum _target, u32 _size, void *_pData, GLbitfield _flags)
		{
			GLbitfield flags = GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
			// potential memcpy to pinned memory if _pData != nullptr
			// prefer initializing with _pData = nullptr then call
			// glMapBufferRange and write the data to the returned pointer
			glBufferStorage(_target, _size, _pData, _flags);
		}

		void invalidate()
		{

		}

		void foo()
		{
			// potential memcpy to pinned memory
			glBufferData(target, size, data, usage);
			glBufferSubData(target, offset, data, data);

			// Re-specification (orphaning)
			glBufferData(target, size, nullptr, usage);
			glMapBufferRange(target, offset, length, GL_MAP_INVALIDATE_BUFFER_BIT || GL_MAP_INVALIDATE_RANGE_BIT);

			// Direct access
			glMapBuffer(target, access);
			glMapBufferRange(target, offset, length, access);
			glFlushMappedBufferRange(target, offset, length);
			glUnmapBuffer(target);

			// Unsynchronized buffer
			glMapBufferRange(target, offset, length, GL_MAP_UNSYNCHRONIZED_BIT);

			// Copy
			glCopyBufferSubData(readTarget, writeTarget, readOffset, writeOffset, size);
		}
	};

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_BUFFERS_STORAGE_POLICIES_HPP */