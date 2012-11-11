#include <stack>

#include "djah/opengl/opengl_logger.hpp"
#include "djah/opengl/vertex_array.hpp"
#include "djah/opengl/vertex_format.hpp"
#include "djah/opengl/vertex_buffer.hpp"
#include "djah/opengl/index_buffer.hpp"
#include "djah/opengl/shader_program.hpp"

#include "djah/debug/log.hpp"

namespace djah { namespace opengl {

	//----------------------------------------------------------------------------------------------
	vertex_array::vertex_array(const vertex_format &format, vertex_buffer *vb, index_buffer *ib)
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
	bool vertex_array::isValidResource() const
	{
		return glIsVertexArray(id_) != 0;
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
	void vertex_array::init(const shader_program &sp) const
	{

		const unsigned int stride = vertex_format_.stride();
		unsigned int offset = 0;

		std::stack<int> stack;

		// Start recording
		bind();

		// Record vertex attributes
		vertex_buffer_->bind();

		const vertex_format::attr_list_t &attributes = vertex_format_.attributes();
		auto it_end(attributes.end());
		for(auto it = attributes.begin(); it != it_end; ++it)
		{
			int attr_index = sp.getVertexAttributeLocation(it->name());
			if( attr_index >= 0 )
			{
				glEnableVertexAttribArray(attr_index);
				DJAH_LOG_TODO("Implement vertex attributes divisor");
				glVertexAttribDivisor(attr_index, it->divisor());
				glVertexAttribPointer(attr_index, it->count(), it->valueType(), GL_FALSE, stride, buffer_offset(offset));
				stack.push(attr_index);
			}
			else
			{
				DJAH_OPENGL_WARNING()
					<< "Attribute \"" << it->name() << "\" not found in the following shader: "
					<< sp.name()
					<< DJAH_END_LOG();
			}
			offset += it->count() * it->size() * (vertex_format_.isPacked() ? vertex_buffer_->size() : 1);
		}

		// Record indices if there's any
		if(index_buffer_)
			index_buffer_->bind();

		// Stop recording
		unbind();

		// Disable vertex attributes
		while(!stack.empty())
		{
			glDisableVertexAttribArray(stack.top());
			stack.pop();
		}

		vertex_buffer_->unbind();
		if(index_buffer_)
			index_buffer_->unbind();

		initialized_ = true;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void vertex_array::draw(int primitiveType) const
	{
		if( initialized_ )
		{
			bind();
			if( index_buffer_ )
			{
				GLsizei count = static_cast<GLsizei>(index_buffer_->count());
				glDrawElements(index_buffer_->drawingMode(), count, index_buffer_->dataType(), 0);
			}
			else
			{
				glDrawArrays(primitiveType, 0, vertex_buffer_->count() *3* 3);
			}
			//unbind();
		}
	}
	//----------------------------------------------------------------------------------------------

} /*opengl*/ } /*djah*/
