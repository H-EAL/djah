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
		check(pVB != nullptr);
		check(vertexBuffers_.find(pVB) == vertexBuffers_.end());
		vertexBuffers_.insert( vb_list_t::value_type(pVB,format) );
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void vertex_array::removeVertexBuffer(vertex_buffer *pVB)
	{
		check(pVB != nullptr);

		auto it = vertexBuffers_.find(pVB);
		check(it != vertexBuffers_.end());

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
		check( !vertexBuffers_.empty() );

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
	void vertex_array::enableVertexBuffer(vertex_buffer *pVB, const vertex_format &vertexFormat, const shader_program &sp, std::stack<int> &attributesStack)
	{
		const unsigned int stride = vertexFormat.stride();
		unsigned int offset = 0;

		const vertex_format::attr_list_t &attributes = vertexFormat.attributes();
		auto attrIt									 = attributes.begin();
		auto attrItEnd								 = attributes.end();

		pVB->bind();

		const shader_program::cache_t &attributeMap = sp.getAttributeMap();
		std::for_each(attributeMap.begin(), attributeMap.end(), [&](const shader_program::cache_t::value_type &attributeInfo)
		{
			auto currentAttributeIt = std::find_if(attrIt, attrItEnd, [&](const opengl::format::vertex_attrib_base &attr)
			{
				return attr.name() == attributeInfo.first;
			});

			if( currentAttributeIt != attrItEnd )
			{
				const auto &currentAttribute = (*currentAttributeIt);

				const unsigned int attrIndex = attributeInfo.second;
				glEnableVertexAttribArray(attrIndex);
				glVertexAttribDivisor(attrIndex, currentAttribute.divisor()); 
				const int attribSizeDiv4 = currentAttribute.count() / 4;
				int i = 0;
				for(i = 0; i < attribSizeDiv4; ++i)
				{
					glVertexAttribPointer(attrIndex + i, 4, currentAttribute.valueType(), GL_FALSE, stride, buffer_offset(offset));
				}
				const int attribSizeMod4 = currentAttribute.count() % 4;
				if( attribSizeMod4 != 0 )
				{
					glVertexAttribPointer(attrIndex + i, attribSizeMod4, currentAttribute.valueType(), GL_FALSE, stride, buffer_offset(offset));
				}

				attributesStack.push(attrIndex);
				offset += currentAttribute.count() * currentAttribute.size() * (vertexFormat.isPacked() ? vertexCount_ : 1);
			}
			else
			{
				DJAH_ASSERT_MSG(false, "[%s] Attribute \"%s\" not found in the vertex format", sp.name().c_str(), attributeInfo.first.c_str());
				return;
			}
		});

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
				glDrawElements(pIndexBuffer_->drawingMode(), count, pIndexBuffer_->dataType(), nullptr);
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
				glDrawElementsInstanced(pIndexBuffer_->drawingMode(), count, pIndexBuffer_->dataType(), nullptr, instanceCount);
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
