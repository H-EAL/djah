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
	vertex_array::vertex_array()
		: pIndexBuffer_(nullptr)
		, vertexCount_(0)
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
		monitor<vertex_array>::bind(id_);
		glBindVertexArray(id_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void vertex_array::unbind()
	{
		monitor<vertex_array>::bind(0);
		glBindVertexArray(0);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void vertex_array::addVertexBuffer(vertex_buffer *pVB, const vertex_format &format)
	{
		DJAH_ASSERT(pVB != nullptr);
		DJAH_ASSERT(vertexBuffers_.find(pVB) == vertexBuffers_.end());
		vertexBuffers_.insert( vb_list_t::value_type(pVB,format) );
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void vertex_array::removeVertexBuffer(vertex_buffer *pVB)
	{
		DJAH_ASSERT(pVB != nullptr);

		auto it = vertexBuffers_.find(pVB);
		DJAH_ASSERT(it != vertexBuffers_.end());

		vertexBuffers_.erase(it);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void vertex_array::setIndexBuffer(index_buffer *pIb)
	{
		pIndexBuffer_ = pIb;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void vertex_array::setVertexCount(int vertexCount)
	{
		vertexCount_ = vertexCount;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void vertex_array::init(const shader_program &sp)
	{
		DJAH_ASSERT( !vertexBuffers_.empty() );

		std::stack<int> attributesStack;

		// Start recording
		bind();

		auto itEnd = vertexBuffers_.end();
		for( auto it = vertexBuffers_.begin(); it != itEnd; ++it )
		{
			enableVertexBuffer(it->first, it->second, sp, attributesStack);
		}

		// Record indices if there's any
		if(pIndexBuffer_)
			pIndexBuffer_->bind();

		// Stop recording
		unbind();

		// Disable vertex attributes
		while(!attributesStack.empty())
		{
			glDisableVertexAttribArray(attributesStack.top());
			attributesStack.pop();
		}

		if(pIndexBuffer_)
			pIndexBuffer_->unbind();

		initialized_ = true;
	}
	//----------------------------------------------------------------------------------------------
	

	//----------------------------------------------------------------------------------------------
	void vertex_array::enableVertexBuffer(vertex_buffer *pVB, const vertex_format &format, const shader_program &sp, std::stack<int> &attributesStack)
	{
		const unsigned int stride = format.stride();
		unsigned int offset = 0;

		const vertex_format::attr_list_t &attributes = format.attributes();

		pVB->bind();

		auto itEnd = attributes.end();
		for(auto it = attributes.begin(); it != itEnd; ++it)
		{
			const int attrIndex = sp.getVertexAttributeLocation(it->name());
			if( attrIndex >= 0 )
			{
				glEnableVertexAttribArray(attrIndex);
				glVertexAttribDivisor(attrIndex, it->divisor());
				const int attribSizeDiv4 = it->count() / 4;
				int i = 0;
				for(i = 0; i < attribSizeDiv4; ++i)
				{
					glVertexAttribPointer(attrIndex + i, 4, it->valueType(), GL_FALSE, stride, buffer_offset(offset));
				}
				const int attribSizeMod4 = it->count() % 4;
				if( attribSizeMod4 != 0 )
				{
					glVertexAttribPointer(attrIndex + i, attribSizeMod4, it->valueType(), GL_FALSE, stride, buffer_offset(offset));
				}
				
				attributesStack.push(attrIndex);
			}
			else
			{
				DJAH_OPENGL_WARNING()
					<< "Attribute \"" << it->name() << "\" not found in the following shader: "
					<< sp.name()
					<< DJAH_END_LOG();
			}
			offset += it->count() * it->size() * (format.isPacked() ? pVB->size() : 1);
		}

		pVB->unbind();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void vertex_array::draw(int primitiveType) const
	{
		DJAH_ASSERT_MSG(initialized_, "Trying to draw an uninitialized vertex_array");

		if( initialized_ )
		{
			bind();
			if( pIndexBuffer_ )
			{
				GLsizei count = static_cast<GLsizei>(pIndexBuffer_->count());
				glDrawElements(pIndexBuffer_->drawingMode(), count, pIndexBuffer_->dataType(), 0);
			}
			else
			{
				glDrawArrays(primitiveType, 0, vertexCount_);
			}
			unbind();
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void vertex_array::drawInstanced(int instanceCount, int primitiveType) const
	{
		DJAH_ASSERT_MSG(initialized_, "Trying to draw an uninitialized vertex_array");

		if( initialized_ )
		{
			bind();
			if( pIndexBuffer_ )
			{
				GLsizei count = static_cast<GLsizei>(pIndexBuffer_->count());
				glDrawElements(pIndexBuffer_->drawingMode(), count, pIndexBuffer_->dataType(), 0);
			}
			else
			{
				glDrawArraysInstanced(primitiveType, 0, vertexCount_, instanceCount);
			}
			unbind();
		}
	}
	//----------------------------------------------------------------------------------------------

} /*opengl*/ } /*djah*/
