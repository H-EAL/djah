#ifndef DJAH_VIDEO_DRIVERS_OPENGL_INCLUDE_HPP
#define DJAH_VIDEO_DRIVERS_OPENGL_INCLUDE_HPP

#include "../../platform.hpp"

#include <GL/glew.h>

#if !DJAH_COMPILE_MACOSX && !DJAH_COMPILE_IPHONEOS
#	include <GL/gl.h>
#	include <GL/glu.h>
#else
#	include <OpenGl/gl.h>
#	include <OpenGl/glu.h>
#endif

#endif /* DJAH_VIDEO_DRIVERS_OPENGL_INCLUDE_HPP */