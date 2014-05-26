#ifndef DJAH_OPENGL_BUFFER_HPP
#define DJAH_OPENGL_BUFFER_HPP

#include "djah/system/gl.hpp"

namespace djah { namespace opengl {

	template<BufferTarget::Type Target>
	struct BufferSelector
	{
		typedef buffer
			<
				Immutable_StoragePolicy,
				Synced_SynchronizationPolicy,
				PermanentMapping_AccessPolicy,
				std::conditional
				<
					gl_contex::is_debug,
					Thorough_ValidationPolicy, 
					Fast_ValidationPolicy
				>::type
			>
			Type;
	};

	typedef BufferSelector<BufferTarget::Array>::Type vertex_buffer;

	namespace interface {
		class buffer
		{
		public:
			virtual void read()  = 0;
			virtual void write() = 0;
		};
	};

	template<typename Policy>
	bool check_policy_specs()
	{
		return gl_context::get_current()->caps().isMinVersion(Policy::MajorVersion, Policy::MinorVersion)
			|| gl_context::get_current()->caps().hasExtensions(Policy::Extensions)
	}
	
	class buffer_proxy
	{
	public:
		buffer_proxy(u32 size)
		{
			if( check_policy_specs<Immutable_StoragePolicy>() )
			{
				pIBuffer_ = new buffer<GL4_Specs>(size);
			}
			else if( check_policy_specs<Mutable_StoragePolicy>() )
			{
				pIBuffer_ = new buffer<GL3_Specs>(size);
			}
			else
			{
				assert(false);
			}
		}

	private:
		interface::buffer *pIBuffer_;
	};

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
	class buffer
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
		buffer(u32 _size);

		// Creates buffer of [sizeof(_array)] bytes
		// Data is initialized with '_array'
		template<typename T, int N>
		buffer(const T (&_array)[N]);		

		// Creates buffer of [_count * sizeof(T)] bytes
		// Data is initialized with '_data'
		template<typename T>
		buffer(u32 _count, const T * const _data);

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

#endif /* DJAH_OPENGL_BUFFER_HPP */