#ifndef DJAH_VIDEO_DRIVERS_OGL_BUFFER_BASE_HPP
#define DJAH_VIDEO_DRIVERS_OGL_BUFFER_BASE_HPP

#include "../resource.hpp"

namespace djah { namespace video { namespace drivers { namespace ogl {

	enum E_BUFFER_USAGE
	{
		EBU_STREAM_DRAW  = GL_STREAM_DRAW,
		EBU_STREAM_READ  = GL_STREAM_READ,
		EBU_STREAM_COPY  = GL_STREAM_COPY,

		EBU_STATIC_DRAW  = GL_STATIC_DRAW,
		EBU_STATIC_READ  = GL_STATIC_READ,
		EBU_STATIC_COPY  = GL_STATIC_COPY,

		EBU_DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
		EBU_DYNAMIC_READ = GL_DYNAMIC_READ,
		EBU_DYNAMIC_COPY = GL_DYNAMIC_COPY
	};

	template<int Target>
	class buffer_base
		: public resource
	{
	public:

		buffer_base(size_t size, E_BUFFER_USAGE usage, bool auto_alloc = true);
		~buffer_base();

		size_t size() const;
		size_t count() const;

		void allocMemory();

		void bind() const;
		static void unbind();

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
		buffer_base<Target>& operator >>(T &toRead);
		template<typename T>
		buffer_base<Target>& operator <<(const T &toWrite);
		void unlock();

	private:

		void aquire();
		void release();

		typedef char byte;

		E_BUFFER_USAGE usage_;
		const size_t   size_;
		size_t         bytes_per_elem_;
		bool           allocated_;
		bool           locked_;
		byte           *cursor_;
		unsigned int   cursor_count_;
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

} /*ogl*/ } /*drivers*/ } /*video*/ } /*djah*/

#include "buffer_base.inl"

#endif /* DJAH_VIDEO_DRIVERS_OGL_BUFFER_BASE_HPP */