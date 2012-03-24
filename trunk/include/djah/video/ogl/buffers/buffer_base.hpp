#ifndef DJAH_VIDEO_OGL_BUFFER_BASE_HPP
#define DJAH_VIDEO_OGL_BUFFER_BASE_HPP

#include <cassert>
#include "../../../types.hpp"
#include "../../../system/opengl/gl.hpp"
#include "../../../system/opengl/types.hpp"
#include "../errors.hpp"
#include "../resource.hpp"

namespace djah { namespace video { namespace ogl {

	enum E_BUFFER_USAGE
	{
		EBU_STREAM_DRAW  = GL_STREAM_DRAW,
		EBU_STATIC_DRAW  = GL_STATIC_DRAW,
		EBU_DYNAMIC_DRAW = GL_DYNAMIC_DRAW
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
		enum E_BUFFER_TARGET
		{
			EBT_VERTEX_BUFFER  = GL_ARRAY_BUFFER,
			EBT_INDEX_BUFFER   = GL_ELEMENT_ARRAY_BUFFER,
			EBT_UNIFORM_BUFFER = GL_UNIFORM_BUFFER
		};

	protected:
		buffer_base(E_BUFFER_TARGET target, E_BUFFER_USAGE usage, size_t size, bool auto_alloc = true);
		virtual ~buffer_base();

	private:
		virtual void aquire();
		virtual void release();
		virtual bool isValidResource() const;

	private:
		E_BUFFER_TARGET	target_;
		E_BUFFER_USAGE	usage_;
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

} /*ogl*/ } /*video*/ } /*djah*/

#include "buffer_base.inl"

#endif /* DJAH_VIDEO_OGL_BUFFER_BASE_HPP */
