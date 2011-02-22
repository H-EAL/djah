#ifndef DJAH_VIDEO_DRIVERS_OPENGL_INCLUDE_HPP
#define DJAH_VIDEO_DRIVERS_OPENGL_INCLUDE_HPP

#include "../../platform.hpp"

#if defined(DJAH_COMPILE_IPHONEOS)
#	include <OpenGlES2/gl.h>
#	include <OpenGlES2/glext.h>
#elif defined(DJAH_COMPILE_MACOSX)
#	include <OpenGl/gl.h>
#	include <OpenGl/glext.h>
#else
#	include "GL/gl.h"
#	include "GL/glext.h"
#endif

#include "ogl/extensions.hpp"

#endif /* DJAH_VIDEO_DRIVERS_OPENGL_INCLUDE_HPP */