#include "system/opengl_include.hpp"
#include "system/ios/opengl_ios.hpp"
#include "system/ios/iosdevice.hpp"

#include "video/ogl/buffers/frame_buffer.hpp"
#include "video/ogl/buffers/render_buffer.hpp"

#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>


//--------------------------------------------------------------------------------------------------
@implementation EAGLView

+ (Class)layerClass {
    return [CAEAGLLayer class];
}

- (void)layoutSubviews
{
	glClearColor(1,1,1,0);
	glDepthRangef(0, 100.0f);
	glViewport(0,0,320,480);
	
	displayLink = [self.window.screen displayLinkWithTarget:self selector:@selector(drawFrame)];
	[displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

- (void)drawFrame {       
	
	djah::system::device_ptr device = djah::system::device_base::get_current();
	djah::system::driver_ptr driver = device->videoDriver();
	
	driver->beginScene();
	glClear(GL_COLOR_BUFFER_BIT);
	
	driver->swapBuffers();
}

@end
//--------------------------------------------------------------------------------------------------



namespace djah { namespace system {
	
	//----------------------------------------------------------------------------------------------
	opengl_context* new_platform_specific_context()
	{
		return new opengl_ios;
	}
	//----------------------------------------------------------------------------------------------
	
	
	//----------------------------------------------------------------------------------------------
	opengl_ios::opengl_ios()
		: eagl_context_(0)
		, view_(0)
		, frame_buffer_(0)
		, render_buffer_(0)
	{
	}
	//----------------------------------------------------------------------------------------------
	
	
	//----------------------------------------------------------------------------------------------
	opengl_ios::~opengl_ios()
	{
	}
	//----------------------------------------------------------------------------------------------
	
	
	//----------------------------------------------------------------------------------------------
	void opengl_ios::create(device_ptr device)
	{
		iosdevice *iosdev = dynamic_cast<iosdevice*>(device);
		assert(iosdev);
		
		view_ = [[EAGLView alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
		assert(view_);
		
		// Add the view to the window
		[iosdev->window_ addSubview:view_];
		
		// Get the layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)view_.layer;
		eaglLayer.opaque = YES;
		eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:NO],kEAGLDrawablePropertyRetainedBacking,kEAGLColorFormatRGBA8,kEAGLDrawablePropertyColorFormat,nil];
        
        eagl_context_ = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
		assert(eagl_context_);
		
		makeCurrent();
		
		frame_buffer_ = new video::ogl::frame_buffer;
		render_buffer_ = new video::ogl::render_buffer(0,0);
		
		frame_buffer_->bind();
		render_buffer_->bind();
		
		[eagl_context_ renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];
		
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, render_buffer_->id());
		
		int backingWidth = 0, backingHeight = 0;
		glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
		glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
	}
	//----------------------------------------------------------------------------------------------
	
	
	//----------------------------------------------------------------------------------------------
	void opengl_ios::destroy()
	{
		delete frame_buffer_;
		delete render_buffer_;
		
		doneCurrent();
		[eagl_context_ release];
	}
	//----------------------------------------------------------------------------------------------
	
	
	//----------------------------------------------------------------------------------------------
	void opengl_ios::swapBuffers()
	{
		render_buffer_->bind();
		[eagl_context_ presentRenderbuffer:GL_RENDERBUFFER];
	}
	//----------------------------------------------------------------------------------------------
	
	
	//----------------------------------------------------------------------------------------------
	void opengl_ios::makeCurrent()
	{
		assert( [EAGLContext setCurrentContext:eagl_context_] == YES );
		if(frame_buffer_) frame_buffer_->bind();
	}
	//----------------------------------------------------------------------------------------------
	
	
	//----------------------------------------------------------------------------------------------
	void opengl_ios::doneCurrent()
	{
        [EAGLContext setCurrentContext:nil];
	}
	//----------------------------------------------------------------------------------------------
	
} /*system*/ } /*djah*/