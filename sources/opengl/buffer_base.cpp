#include "djah/opengl/buffer_base.hpp"

namespace djah { namespace opengl {

	//----------------------------------------------------------------------------------------------
	buffer_base::buffer_base(eBufferTarget target, eBufferUsage usage, size_t size, bool auto_alloc)
		: target_(target)
		, usage_(usage)
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
	buffer_base::~buffer_base()
	{
		release();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void buffer_base::aquire()
	{
		glGenBuffers(1, &id_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void buffer_base::release()
	{
		glDeleteBuffers(1, &id_);
		id_ = INVALID_ID;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool buffer_base::isValidResource() const
	{
		return glIsBuffer(id_) != 0;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void buffer_base::bind() const
	{
		assert(id_ != INVALID_ID);
		glBindBuffer(target_, id_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void buffer_base::unbind()
	{
		glBindBuffer(target_, 0);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	size_t buffer_base::size() const
	{
		return size_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	size_t buffer_base::count() const
	{
		return size_ / bytes_per_elem_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	u32 buffer_base::dataType() const
	{
		return data_type_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void buffer_base::allocMemory()
	{
		if( allocated_ )
			return;

		bind();
		glBufferData(target_, size_, 0, usage_);
		unbind();

		allocated_ = true;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void buffer_base::lock()
	{
		//bind();
		//cursor_ = reinterpret_cast<byte*>(glMapBuffer(Target, GL_WRITE_ONLY));
		//cursor_count_ = 0;
		//locked_ = true;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void buffer_base::unlock()
	{
		//glUnmapBuffer(Target);
		//unbind();
		//cursor_ = 0;
		//cursor_count_ = 0;
		//locked_ = false;
	}
	//----------------------------------------------------------------------------------------------

} /*opengl*/ } /*djah*/
