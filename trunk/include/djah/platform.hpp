#ifndef DJAH_PLATFORM_HPP
#define DJAH_PLATFORM_HPP

#if defined(_WIN32)
#	define DJAH_COMPILE_WINDOWS
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>

#elif defined(linux)
#	define DJAH_COMPILE_LINUX

#elif defined(TARGET_OS_MAC)
#	define DJAH_COMPILE_MACOSX

#elif defined(IPHONEOS)
#	define DJAH_COMPILE_IPHONEOS

#else
#	define DJAH_UNKNOWN_PLATFORM
#endif

#endif /* DJAH_PLATFORM_HPP */