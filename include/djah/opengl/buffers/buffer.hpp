#ifndef DJAH_OPENGL_BUFFER_HPP
#define DJAH_OPENGL_BUFFER_HPP

#include <memory>

#include "djah/types.hpp"
#include "djah/system/gl.hpp"
#include "djah/opengl/buffers/interface_buffer.hpp"

namespace djah { namespace opengl {

    //////////////////////////////////////////////////////////////////////////
    template<typename Policy>
    bool check_policy_specs()
    {
        return gl_context::get_current()->caps().isMinVersion(Policy::MajorVersion, Policy::MinorVersion)
            || gl_context::get_current()->caps().hasExtensions(Policy::Extensions)
    }

    class buffer_allocator
    {
    public:
        std::unique_ptr<interface::buffer> create()
        {
            
        }
    };
    
    //////////////////////////////////////////////////////////////////////////
    class buffer
        : public interface::buffer
    {
    public:
        buffer(u32 _size)
            : upIBuffer_( create_new_buffer(_size) )
            , size_(_size)
        {
        }

        size_t size() const
        {
            return size_;
        }

        template<typename T>
        size_t read(T &_data);
        template<typename T, size_t N>
        size_t read(T (&_vData)[N]);
        template<typename T>
        size_t read(T *_pData, size_t _count);

        template<typename T>
        size_t write(const T &_data);
        template<typename T, size_t N>
        size_t write(const T (&_vData)[N]);
        template<typename T>
        size_t write(const T * const _pData, size_t _count);

        template<typename T>
        buffer& operator >>(T &_toRead);
        template<typename T>
        buffer& operator <<(const T &_toWrite);

    private:
        static interface::buffer* create_new_buffer(size_t _size)
        {            
            /*
            if( check_policy_specs<Immutable_StoragePolicy>() )
            {
                upIBuffer_ = new buffer_storage<GL4_Specs>(size);
            }
            else if( check_policy_specs<Mutable_StoragePolicy>() )
            {
                upIBuffer_ = new buffer_storage<GL3_Specs>(size);
            }
            else
            {
                assert(false);
            }
            */
        }

    private:
        inline virtual size_t read(void *_pDst, size_t _size) override
        {
            return upIBuffer_->read(_pDst, _size);
        }

        inline virtual size_t write(const void * const _pDst, size_t _size) override
        {
            return upIBuffer_->write(_pDst, _size);
        }

    private:
        std::unique_ptr<interface::buffer> upIBuffer_;
        size_t                             size_;
    };

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_BUFFER_HPP */