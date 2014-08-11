#ifndef DJAH_OPENGL_BUFFER_STORAGE_HPP
#define DJAH_OPENGL_BUFFER_STORAGE_HPP

#include "djah/types.hpp"
#include "djah/opengl/buffers/interface_buffer.hpp"

namespace djah { namespace opengl {

    //////////////////////////////////////////////////////////////////////////
    // 1 - Allocate
    // 2 - Use
    //		a - read
    //		b - write
    // 3 - Destroy
    template
    <
        typename StoragePolicy,
        typename BufferingPolicy,
        typename AccessPolicy,
        typename ValidationPolicy
    >
    class buffer_storage
        : public interface::buffer
        , public resource
        , public StoragePolicy
        , public BufferingPolicy
        , public AccessPolicy
        , public ValidationPolicy
    {
    public:
        // Use default target BufferTarget::CopyWrite
        // Creates buffer of [_size] bytes
        // Data is left uninitialized
        buffer_storage(u32 _size);

        // Creates buffer of [sizeof(_array)] bytes
        // Data is initialized with '_array'
        template<typename T, int N>
        buffer_storage(const T (&_vArray)[N]);		

        // Creates buffer of [_count * sizeof(T)] bytes
        // Data is initialized with '_pData'
        template<typename T>
        buffer_storage(u32 _count, const T * const _pData);

        virtual size_t read(void *_pDst, size_t _size) override
        {
            return StoragePolicy::read(_pDst, _size);
        }

        virtual size_t write(const void * const _pDst, size_t _size) override
        {
            return StoragePolicy::write(_pDst, _size);
        }

    private:
        virtual void aquire();
        virtual void release();
        virtual bool isValidResource() const;

        void create(u32 _size)
        {
            static const GLenum PreferredTarget = AccessPolicy::PreferredTarget;
            static const bool isReadable = AccessPolicy::ReadAccess;
            static const bool isWritable = AccessPolicy::WriteAccess;

            StoragePolicy::allocateStorage(id_, PreferredTarget, _size, isReadable, isWritable);
        }

    private:
        u32 size_;
    };

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_BUFFER_STORAGEHPP */