#ifndef DJAH_OPENGL_BUFFER_BASE_HPP
#define DJAH_OPENGL_BUFFER_BASE_HPP

#include <cassert>
#include "types.hpp"
#include "system/gl.hpp"
#include "gl_types.hpp"
#include "errors.hpp"
#include "resource.hpp"

namespace djah { namespace opengl {

	enum eBufferUsage
	{
		eBU_StreamDraw  = GL_STREAM_DRAW,
		eBU_StaticDraw  = GL_STATIC_DRAW,
		eBU_DynamicDraw = GL_DYNAMIC_DRAW
	};

	class buffer_base
		: public resource
	{
	public:
		size_t size() const;
		size_t count() const;
		u32	   dataType() const;	

		void allocMemory();

		void bind() const;
		void unbind();

		template<typename T>
		size_t read(T *data_ptr, size_t count, int offset = 0);
		template<typename T, size_t N>
		size_t read(T (&data_array)[N], int offset = 0);

		template<typename T>
		size_t write(const T *data_ptr, size_t count, int offset = 0);
		template<typename T, size_t N>
		size_t write(const T (&data_array)[N], int offset = 0);

		void lock();
		template<typename T>
		buffer_base& operator >>(T &toRead);
		template<typename T>
		buffer_base& operator <<(const T &toWrite);
		void unlock();

	protected:
		enum eBufferTarget
		{
			eBT_VertexBuffer  = GL_ARRAY_BUFFER,
			eBT_IndexBuffer   = GL_ELEMENT_ARRAY_BUFFER,
			eBT_UniformBuffer = GL_UNIFORM_BUFFER
		};

	protected:
		buffer_base(eBufferTarget target, eBufferUsage usage, size_t size, bool auto_alloc = true);
		virtual ~buffer_base();

	private:
		virtual void aquire();
		virtual void release();
		virtual bool isValidResource() const;

	private:
		eBufferTarget	target_;
		eBufferUsage	usage_;
		const size_t	size_;
		size_t			bytes_per_elem_;
		u32				data_type_;
		bool			allocated_;
		bool			locked_;
		byte			*cursor_;
		u32				cursor_count_;
	};


	inline void* buffer_offset(size_t offset)
	{
		return reinterpret_cast<void*>(offset);
	}

	template<typename T>
	inline void* buffer_offset(size_t offset)
	{
		return reinterpret_cast<void*>(offset * sizeof(T));
	}

} /*opengl*/ } /*djah*/

#include "buffer_base.inl"

#endif /* DJAH_OPENGL_BUFFER_BASE_HPP */
