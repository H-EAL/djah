#include "system/linux/x11device.hpp"

#include "platform.hpp"

#include <cassert>
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <GL/glx.h>

#include "geometry/rect.hpp"


namespace djah { namespace system {

	//----------------------------------------------------------------------------------------------
	device_ptr new_platform_specific_device()
	{
		return new x11device;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	x11device::x11device()
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	x11device::~x11device()
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void x11device::createImpl()
	{
        display_ = XOpenDisplay(0);
        assert(display_);

        GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, video_config_.depthBits, GLX_DOUBLEBUFFER, None };
        XVisualInfo *visualInfo = glXChooseVisual(display_, 0, att);
        assert(visualInfo);

        Window rootWindow = DefaultRootWindow(display_);
        Colormap colorMap = XCreateColormap(display_, rootWindow, visualInfo->visual, AllocNone);

		XSetWindowAttributes attributes;
        attributes.colormap = colorMap;
        attributes.event_mask = ExposureMask | KeyPressMask;

		window_  = XCreateWindow
		(
			display_,
			rootWindow,
			0, 0, 
			video_config_.width, video_config_.height,
			0,
			visualInfo->depth,
			InputOutput,
			visualInfo->visual,
			CWColormap | CWEventMask,
			&attributes
		);
        assert(window_);

        show();

        setWindowTitle("Djah's Heavenly Window");

        glx_context_ = glXCreateContext(display_, visualInfo, 0, GL_TRUE);
        assert(glx_context_);
        glXMakeCurrent(display_, window_, glx_context_);

        driver_->create();
	}
	//----------------------------------------------------------------------------------------------
	
	
	//----------------------------------------------------------------------------------------------
	void x11device::destroyImpl()
	{
        glXMakeCurrent(display_, None, 0);
 		glXDestroyContext(display_, glx_context_);
 		XDestroyWindow(display_, window_);
 		XCloseDisplay(display_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void x11device::show()
	{
        XMapWindow(display_, window_);
	}
	//----------------------------------------------------------------------------------------------
	
	
	//----------------------------------------------------------------------------------------------
	void x11device::swapBuffers()
	{
        glXSwapBuffers(display_, window_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool x11device::isWindowActive()
	{
		return true;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool x11device::hasWindowFocus()
	{
		return true;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void x11device::setWindowTitle(const std::string &title)
	{
        XStoreName(display_, window_, title.c_str());
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool x11device::runImpl()
	{
        XEvent event;

        while( XPending(display_) > 0 )
        {
            XNextEvent(display_, &event);
            /*switch( event.type )
            {
            case ResizeRequest:
                resize(event.xresizerequest.width, event.xresizerequest.height);
                break;

            case KeyPress:
                if(event.xkey.keycode == 32)
                    shutDown();
                break;
            }*/
        }

		return true;
	}
	//----------------------------------------------------------------------------------------------

} /*system*/ } /*djah*/