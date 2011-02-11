#ifndef DJAH_PLATFORM_HPP
#define DJAH_PLATFORM_HPP

#define DEFINE_DEVICE_IMPL(device) typedef djah::video::devices::##device			DeviceImpl
#define DEFINE_DRIVER_IMPL(driver) typedef djah::video::drivers::detail::##driver	OglImpl

#define TARGET_OS_MAC

#if defined(_WIN32)
#	define DJAH_COMPILE_WINDOWS
#	include <windows.h>
#	include "video/devices/win32device.hpp"
#	include "video/drivers/detail/opengl_win32.hpp"
	DEFINE_DEVICE_IMPL(win32device);
	DEFINE_DRIVER_IMPL(opengl_win32);

#elif defined(linux)
#	define DJAH_COMPILE_LINUX
#	include "video/devices/x11device.hpp"
#	include "video/drivers/detail/opengl_x11.hpp"
	DEFINE_DEVICE_IMPL(x11device);
	DEFINE_DRIVER_IMPL(opengl_x11);

#elif defined(TARGET_OS_MAC)
//#	define DJAH_COMPILE_MACOSX
#	define DJAH_COMPILE_LINUX
#	include "video/devices/x11device.hpp"
#	include "video/drivers/detail/opengl_x11.hpp"
typedef djah::video::devices::x11device			DeviceImpl;
typedef djah::video::drivers::detail::opengl_x11	OglImpl;

#elif defined(IPHONEOS)
#	define DJAH_COMPILE_IPHONEOS

#else
#	define DJAH_UNKNOWN_PLATFORM
#endif


// OpenGL includes
#ifdef DJAH_COMPILE_WINDOWS
#	include <windows.h>
#endif

#include <GL/glew.h>

#ifndef TARGET_OS_MAC
#	include <GL/gl.h>
#	include <GL/glu.h>
#else
#	include <OpenGl/gl.h>
#	include <OpenGl/glu.h>
#endif



#undef DEFINE_DEVICE_IMPL
#undef DEFINE_DRIVER_IMPL

#endif /* DJAH_PLATFORM_HPP */