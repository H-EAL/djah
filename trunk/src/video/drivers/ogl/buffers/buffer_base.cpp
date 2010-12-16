#include <cassert>
#include <GL/glew.h>

#include "video/drivers/ogl/buffers/buffer_base.hpp"
#include "video/drivers/ogl/errors.hpp"

namespace djah { namespace video { namespace drivers { namespace ogl {

	//----------------------------------------------------------------------------------------------
	template<int Target>
	buffer_base<Target>::buffer_base(size_t size, E_BUFFER_USAGE usage, bool auto_alloc)
		: usage_(usage)
		, size_(size)
		, bytes_per_elem_(0)
		, data_type_(0)
		, allocated_(false)
	{
		aquire();
		if( auto_alloc )
			allocMemory();
	}
	//----------------------------------------------------------------------------------------------
	

	//----------------------------------------------------------------------------------------------
	template<int Target>
	buffer_base<Target>::~buffer_base()
	{
		release();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int Target>
	void buffer_base<Target>::aquire()
	{
		DJAH_TEST_FOR_OPENGL_ERRORS
		(
			glGenBuffers(1, &id_)
		);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int Target>
	void buffer_base<Target>::release()
	{
		glDeleteBuffers(1, &id_);
		id_ = INVALID_ID;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int Target>
	void buffer_base<Target>::bind() const
	{
		assert(id_ != INVALID_ID);
		glBindBuffer(Target, id_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int Target>
	void buffer_base<Target>::unbind()
	{
		glBindBuffer(Target, 0);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int Target>
	size_t buffer_base<Target>::size() const
	{
		return size_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int Target>
	size_t buffer_base<Target>::count() const
	{
		return size_ / bytes_per_elem_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int Target>
	size_t buffer_base<Target>::dataType() const
	{
		return dataType();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int Target>
	void buffer_base<Target>::allocMemory()
	{
		if( allocated_ )
			return;

		bind();
		glBufferData(Target, size_, 0, usage_);
		unbind();

		allocated_ = true;
	}
	//----------------------------------------------------------------------------------------------

} /*ogl*/ } /*drivers*/ } /*video*/ } /*djah*/