#ifndef DJAH_SYSTEM_OPENGL_GL_HPP
#define DJAH_SYSTEM_OPENGL_GL_HPP

#include "djah/platform.hpp"

//#define DJAH_COMPILE_IPHONEOS 1

#if defined(DJAH_COMPILE_IPHONEOS)
#	include <OpenGlES/ES2/gl.h>
#	include <OpenGlES/ES2/glext.h>
#elif defined(DJAH_COMPILE_MACOSX)
#	include <OpenGl/gl.h>
#	include <OpenGl/glext.h>
#elif defined(DJAH_COMPILE_LINUX)
#	include <GL/gl.h>
#	include <GL/glx.h>
#elif defined(DJAH_COMPILE_WINDOWS)
#	include "GL4/gl.h"
#	include "GL4/glext.h"
#	include "GL4/wglext.h"
#endif

#ifndef DJAH_COMPILE_IPHONEOS
#	include "extensions.hpp"
#endif

#endif /* DJAH_SYSTEM_OPENGL_GL_HPP */