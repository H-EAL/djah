#ifndef DJAH_OPENGL_BUFFER_VIEW_HPP
#define DJAH_OPENGL_BUFFER_VIEW_HPP

#include <memory>

#include "djah/types.hpp"
#include "djah/debug/assertion.hpp"
#include "djah/system/gl.hpp"
#include "djah/opengl/buffers/interface_buffer.hpp"
#include "djah/opengl/buffers/buffer.hpp"

namespace djah { namespace opengl {

	class buffer_view
		: public interface::buffer
	{
	public:
		buffer_view(const std::shared_ptr<buffer> &_spBuffer, size_t _offset, size_t _size)
			: spBuffer_(_spBuffer)
			, offset_(_offset)
			, size_(_size)
		{
			check(_offset + _size <= _spBuffer->size());
		}

		virtual size_t size()
		{
			return size_;
		}

		virtual size_t read()
		{
			return spBuffer_->read();
		}

		virtual size_t write()
		{
			return spBuffer_->write();
		}

	private:
		std::shared_ptr<buffer> spBuffer_;
		size_t offset_;
		size_t size_;
	};

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_BUFFER_VIEW_HPP */