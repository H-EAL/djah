#ifndef DJAH_SYSTEM_OPENGL_CONTEXT_HPP
#define DJAH_SYSTEM_OPENGL_CONTEXT_HPP

#include "device_base.hpp"

namespace djah { namespace system {
	
	//----------------------------------------------------------------------------------------------
	class opengl_context
	{
	public:

		opengl_context() {}
		virtual ~opengl_context() {}

		virtual void create(device_ptr device) = 0;
		virtual void destroy() = 0;
		virtual void swapBuffers() = 0;

		virtual void makeCurrent() = 0;
		virtual void doneCurrent() = 0;
	};
	//----------------------------------------------------------------------------------------------
	
	
	//----------------------------------------------------------------------------------------------
	// Prototype of the actual instance creator (platform specific)
	//----------------------------------------------------------------------------------------------
	opengl_context* new_platform_specific_context();
	//----------------------------------------------------------------------------------------------

} /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_OPENGL_CONTEXT_HPP */
