#ifndef DJAH_PLATFORM_HPP
#define DJAH_PLATFORM_HPP

#if defined(_WIN32)
#   define DJAH_COMPILE_WINDOWS
#   define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#	include <MMSystem.h>
#	include <DbgHelp.h>
#	include <Xinput.h>
#	ifdef near
#		undef near
#	endif
#	ifdef far
#		undef far
#	endif
#	ifdef min
#		undef min
#	endif
#	ifdef max
#		undef max
#	endif

#elif defined(linux)
#   define DJAH_COMPILE_LINUX
#   include<X11/X.h>
#   include<X11/Xlib.h>

#elif defined(TARGET_OS_MAC)
#	define DJAH_COMPILE_MACOSX

#elif defined(IPHONEOS)
#	define DJAH_COMPILE_IPHONEOS

#else
#	define DJAH_UNKNOWN_PLATFORM
#endif

#endif /* DJAH_PLATFORM_HPP */