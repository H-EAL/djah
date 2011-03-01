#ifndef DJAH_SYSTEM_OPENGL_INCLUDE_HPP
#define DJAH_SYSTEM_OPENGL_INCLUDE_HPP

#include "../platform.hpp"

#define DJAH_COMPILE_IPHONEOS 1

#if defined(DJAH_COMPILE_IPHONEOS)
#	include <OpenGlES/ES2/gl.h>
#	include <OpenGlES/ES2/glext.h>
#elif defined(DJAH_COMPILE_MACOSX)
#	include <OpenGl/gl.h>
#	include <OpenGl/glext.h>
#else
#	include "GL/gl.h"
#	include "GL/glext.h"
#endif

#ifndef DJAH_COMPILE_IPHONEOS
#	include "../video/ogl/extensions.hpp"
#endif

#endif /* DJAH_SYSTEM_OPENGL_INCLUDE_HPP */