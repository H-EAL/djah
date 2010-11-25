#ifndef DJAH_PLATFORM_HPP
#define DJAH_PLATFORM_HPP

#define DEFINE_DEVICE_IMPL(device) typedef djah::video::devices::##device			DeviceImpl
#define DEFINE_DRIVER_IMPL(driver) typedef djah::video::drivers::detail::##driver	OglImpl


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

#elif defined(MACOSX)
#	define DJAH_COMPILE_MACOSX

#elif defined(IPHONEOS)
#	define DJAH_COMPILE_IPHONEOS

#else
#	define DJAH_UNKNOWN_PLATFORM
#endif


#undef DEFINE_DEVICE_IMPL
#undef DEFINE_DRIVER_IMPL

#endif /* DJAH_PLATFORM_HPP */