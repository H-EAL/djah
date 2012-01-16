#ifndef DJAH_SYSTEM_IOS_IOSDEVICE_HPP
#define DJAH_SYSTEM_IOS_IOSDEVICE_HPP

#include "../device_base.hpp"

#import <UIKit/UIKit.h>

@class EAGLContext;
@class UIWindow;
@class UINavigationController;
@class EAGLView2;
@class CADisplayLink;

// iPhone entry point
@interface DjahMainApp : UIApplication
{
}
@end

namespace djah { namespace system {
	
	// Forward declaration
	class opengl_ios;
		
	class iosdevice
		: public device_base
	{
	public:
		
		iosdevice();
		~iosdevice() {}
		
		virtual void show() {}
		virtual bool isWindowActive() { return true; }
		virtual bool hasWindowFocus() { return true; }
		
		virtual void setWindowTitle(const std::string &title) {}
		
	private:
		
		virtual void createImpl();
		virtual void destroyImpl();
		virtual bool runImpl();
		
		// Platform specific
		friend class opengl_ios;
		UIWindow *window_;
	};
	
} /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_IOS_IOSDEVICE_HPP */