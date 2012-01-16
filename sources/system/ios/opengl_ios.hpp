#ifndef DJAH_SYSTEM_IOS_OPENGL_IOS_HPP
#define DJAH_SYSTEM_IOS_OPENGL_IOS_HPP

#include "../opengl_context.hpp"

#import <UIKit/UIKit.h>

@class EAGLContext;
@class CADisplayLink;

@interface EAGLView: UIView {
    
	@private
	
	CADisplayLink *displayLink;
	
}
- (void)drawFrame;

@end


namespace djah { namespace video { namespace ogl {
	class frame_buffer;
	class render_buffer;
} /*ogl*/ } /*video*/ } /*djah*/

namespace djah { namespace system {
	
	
	class opengl_ios
		: public opengl_context
	{
	public:
		
		opengl_ios();
		virtual ~opengl_ios();
		
		virtual void create(device_ptr device);
		virtual void destroy();
		virtual void swapBuffers();
		
		virtual void makeCurrent();
		virtual void doneCurrent();
		
	private:
		
		EAGLView *view_;
		EAGLContext *eagl_context_;
		video::ogl::frame_buffer *frame_buffer_;
		video::ogl::render_buffer *render_buffer_;
	};
	
} /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_IOS_OPENGL_IOS_HPP */