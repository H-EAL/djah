#ifndef DJAH_OPENGL_RING_BUFFER_HPP
#define DJAH_OPENGL_RING_BUFFER_HPP

#include <memory>

#include "djah/types.hpp"
#include "djah/system/gl.hpp"

namespace djah { namespace opengl {

	template
	<
		int BufferCount,
		typename SyncronisationPolicy
	>
	class ring_buffer
	{
	public:
		void advance()
		{
			// lock bufferViews_[currentBufferId_]
			currentBufferId_ = (currentBufferId_ + 1) % N;
			// wait for bufferViews_[currentBufferId_] lock
		}

		const buffer_view& currentView() const
		{
			return bufferViews_[currentBufferId_];
		}

	private:
		buffer_view bufferViews_[N];
		u32 currentBufferId_;
	};

	typedef ring_buffer<2> double_buffer;
	typedef ring_buffer<3> triple_buffer;

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_RING_BUFFER_HPP */