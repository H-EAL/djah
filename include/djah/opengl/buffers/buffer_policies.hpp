#ifndef DJAH_OPENGL_BUFFER_POLICIES_HPP
#define DJAH_OPENGL_BUFFER_POLICIES_HPP

#include "djah/system/gl.hpp"
#include "djah/debug/assertion.hpp"
#include "djah/opengl/enum_buffer_target.hpp"
#include "djah/opengl/enum_buffer_usage.hpp"


namespace djah { namespace opengl {

	template<bool _ReadAccess, bool _WriteAccess>
	class CoherentPersistent_AccessPolicy
	{
	protected:
		static const bool ReadAccess  = _ReadAccess;
		static const bool WriteAccess = _WriteAccess;

	protected:
		CoherentPersistent_AccessPolicy()
			: pBufferHead_(nullptr)
		{
		}

		~CoherentPersistent_AccessPolicy()
		{
			glUnmapBuffer();
			pBufferHead_ = nullptr;
		}
		
		void * map(GLenum _target, u32 _size)
		{
			if( pBufferHead_ == nullptr )
			{
				static const u32 flags = 
					  GL_MAP_COHERENT_BIT
					| GL_MAP_PERSISTENT_BIT
					| ReadAccess  ? GL_MAP_READ_BIT  : 0
					| WriteAccess ? GL_MAP_WRITE_BIT : 0;

				pBufferHead_ = glMapBufferRange(_target, 0, _size, flags);
			}

			return pBufferHead_;
		}

		void unmap()
		{
			// do nothing
		}

	private:
		void *pBufferHead_;
	};

	class Thorough_ValidationPolicy
	{
	protected:

		bool validateStorageFlags(GLbitfield _storageFlags) const
		{
			bool isValid = true;

			const bool isDynamicStorage    = (_storageFlags & GL_DYNAMIC_STORAGE_BIT) != 0;
			const bool isStorageReadable   = (_storageFlags & GL_MAP_READ_BIT) != 0;
			const bool isStorageWritable   = (_storageFlags & GL_MAP_WRITE_BIT) != 0;
			const bool isStoragePersistent = (_storageFlags & GL_MAP_PERSISTENT_BIT) != 0;
			const bool isStorageCoherent   = (_storageFlags & GL_MAP_COHERENT_BIT) != 0;
			const bool isClientStorage     = (_storageFlags & GL_MAP_COHERENT_BIT) != 0;

			if( isStoragePersistent )
			{
				isValid &= (isStorageReadable || isStorageWritable);
			}

			if( isStorageCoherent )
			{
				isValid &= isStoragePersistent;
			}

			return isValid;
		}

		bool validateAccessFlags(GLbitfield _accessFlags) const
		{
			bool isValid = true;

			const bool isAccessReadable   = (_accessFlags & GL_MAP_READ_BIT) != 0;
			const bool isAccessWritable   = (_accessFlags & GL_MAP_WRITE_BIT) != 0;
			const bool isAccessPersistent = (_accessFlags & GL_MAP_PERSISTENT_BIT) != 0;

			isValid &= (isAccessReadable || isAccessWritable);

			return isValid;
		}

		bool validateStorageAccess(GLbitfield _storageFlags, GLbitfield _accessFlags) const
		{
			bool isValid = true;

			const bool isDynamicStorage    = (_storageFlags & GL_DYNAMIC_STORAGE_BIT) != 0;
			const bool isStorageReadable   = (_storageFlags & GL_MAP_READ_BIT) != 0;
			const bool isStorageWritable   = (_storageFlags & GL_MAP_WRITE_BIT) != 0;
			const bool isStoragePersistent = (_storageFlags & GL_MAP_PERSISTENT_BIT) != 0;
			const bool isStorageCoherent   = (_storageFlags & GL_MAP_COHERENT_BIT) != 0;
			const bool isClientStorage     = (_storageFlags & GL_MAP_COHERENT_BIT) != 0;

			const bool isAccessReadable   = (_accessFlags & GL_MAP_READ_BIT) != 0;
			const bool isAccessWritable   = (_accessFlags & GL_MAP_WRITE_BIT) != 0;
			const bool isAccessPersistent = (_accessFlags & GL_MAP_PERSISTENT_BIT) != 0;

			if( isAccessReadable )
			{
				isValid &= isStorageReadable;
			}

			if( isAccessWritable )
			{
				isValid &= isStorageWritable;
			}

			return isValid;
		}
	};

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_BUFFER_POLICIES_HPP */