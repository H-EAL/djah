#ifndef DJAH_OPENGL_BUFFER_HPP
#define DJAH_OPENGL_BUFFER_HPP

#include <memory>

#include "djah/types.hpp"
#include "djah/system/gl.hpp"

namespace djah { namespace opengl {

	//////////////////////////////////////////////////////////////////////////
	template<typename Policy>
	bool check_policy_specs()
	{
		return gl_context::get_current()->caps().isMinVersion(Policy::MajorVersion, Policy::MinorVersion)
			|| gl_context::get_current()->caps().hasExtensions(Policy::Extensions)
	}
	
	//////////////////////////////////////////////////////////////////////////
	class buffer
		: public interface::buffer
	{
	public:
		buffer(u32 size)
		{
			if( check_policy_specs<Immutable_StoragePolicy>() )
			{
				pIBuffer_ = new buffer_storage<GL4_Specs>(size);
			}
			else if( check_policy_specs<Mutable_StoragePolicy>() )
			{
				pIBuffer_ = new buffer_storage<GL3_Specs>(size);
			}
			else
			{
				assert(false);
			}
		}

		virtual u32 size()
		{
			return pIBuffer_->size();
		}

		virtual void read()
		{
			return pIBuffer_->read();
		}

		virtual void write()
		{
			return pIBuffer_->write();
		}

		template<typename T>
		size_t read(T &data);
		template<typename T, size_t N>
		size_t read(T (&data_array)[N]);
		template<typename T>
		size_t read(T *data_ptr, size_t count);

		template<typename T>
		size_t write(const T &data);
		template<typename T, size_t N>
		size_t write(const T (&vData)[N]);
		template<typename T>
		size_t write(const T *pData, size_t count);

		template<typename T>
		buffer& operator >>(T &toRead);
		template<typename T>
		buffer& operator <<(const T &toWrite);

	private:
		interface::buffer *pIBuffer_;
	};

	template
	<
		int BufferCount,
		typename SyncronisationPolicy
	>
	class ring_buffer
	{
	public:
		void advance()
		{
			// lock bufferViews_[currentBufferId_]
			currentBufferId_ = (currentBufferId_ + 1) % N;
			// wait for bufferViews_[currentBufferId_] lock
		}

		const buffer_view& currentView() const
		{
			return bufferViews_[currentBufferId_];
		}

	private:
		buffer_view bufferViews_[N];
		u32 currentBufferId_;
	};

	typedef ring_buffer<2> double_buffer;
	typedef ring_buffer<3> triple_buffer;

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_BUFFER_HPP */