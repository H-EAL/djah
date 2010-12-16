#include <stack>

#include "video/drivers/ogl/buffers/vertex_array.hpp"

#include "video/drivers/ogl/buffers/vertex_format.hpp"
#include "video/drivers/ogl/buffers/vertex_buffer.hpp"
#include "video/drivers/ogl/buffers/index_buffer.hpp"

#include "video/drivers/ogl/shaders/shader.hpp"

#include "log/logger.hpp"

namespace djah { namespace video { namespace drivers { namespace ogl {

	//----------------------------------------------------------------------------------------------
	vertex_array::vertex_array(const vertex_buffer &vb, const index_buffer &ib, const vertex_format &format)
		: vertex_format_(format)
		, vertex_buffer_(vb)
		, index_buffer_(ib)
		, initialized_(false)
	{
		DJAH_LOG_TODO("Make vertex_array attributes shared pointers");
		aquire();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	vertex_array::~vertex_array()
	{
		release();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void vertex_array::aquire()
	{
		glGenVertexArrays(1, &id_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void vertex_array::release()
	{
		glDeleteVertexArrays(1, &id_);
		id_ = INVALID_ID;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void vertex_array::bind() const
	{
		glBindVertexArray(id_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void vertex_array::unbind()
	{
		glBindVertexArray(0);
	}
	//----------------------------------------------------------------------------------------------
	

	//----------------------------------------------------------------------------------------------
	void vertex_array::init(const shader &sp) const
	{
		const vertex_format::attr_list_t &attributes = vertex_format_.attributes();
		vertex_format::attr_list_t::const_iterator it;
		vertex_format::attr_list_t::const_iterator it_end(attributes.end());

		unsigned int stride = vertex_format_.stride();
		unsigned int offset = 0;

		std::stack<int> stack;

		// Start recording
		bind();

		// Record vertex attributes
		vertex_buffer_.bind();
		for(it = attributes.begin(); it != it_end; ++it)
		{
			int attr_index = sp.getVertexAttributeLocation(it->name());
			if( attr_index >= 0 )
			{
				glEnableVertexAttribArray(attr_index);
				glVertexAttribPointer(attr_index, it->count(), it->valueType(), GL_FALSE, stride, buffer_offset(offset));
				offset += it->count() * it->size() * (vertex_format_.isPacked() ? vertex_buffer_.size() : 1);
				stack.push(attr_index);
			}
		}

		// Record indices
		index_buffer_.bind();

		// Stop recording
		unbind();

		// Disable vertex attributes
		while(!stack.empty())
		{
			glDisableVertexAttribArray(stack.top());
			stack.pop();
		}

		vertex_buffer_.unbind();
		index_buffer_.unbind();

		initialized_ = true;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void vertex_array::draw() const
	{
		if( initialized_ )
		{
			bind();
			GLsizei count = static_cast<GLsizei>(index_buffer_.count());
			glDrawElements(index_buffer_.drawingMode(), count, index_buffer_.dataType(), 0);
			//unbind();
		}
	}
	//----------------------------------------------------------------------------------------------

} /*ogl*/ } /*drivers*/ } /*video*/ } /*djah*/