#ifndef DJAH_OPENGL_GL_ENUM_HPP
#define DJAH_OPENGL_GL_ENUM_HPP

#include "djah/types.hpp"
#include "djah/core/murmur_hash.hpp"
#include "djah/system/gl.hpp"


#define DJAH_BUILD_ENUM(ENUM_TYPE, ENUM_VAL, GLENUM, MAJOR, MINOR)\
	ENUM_VAL,

#define DJAH_BUILD_GL_ENUM_RUNTIME_LOOK_UP(ENUM_TYPE, ENUM_VAL, GLENUM, MAJOR, MINOR)\
	{																			\
		GLENUM,																	\
		djah_static_murmur_hash(#GLENUM),										\
		djah_static_murmur_hash(#ENUM_TYPE ##"::"#ENUM_VAL),					\
		#GLENUM,																\
		#ENUM_TYPE ##"::"#ENUM_VAL,												\
		MAJOR,																	\
		MINOR																	\
	},

#define DJAH_BUILD_GL_ENUM_COMPILETIME_LOOK_UP(ENUM_TYPE, ENUM_VAL, GLENUM, MAJOR, MINOR)\
	template<> struct ToGL<ENUM_VAL>											\
	{																			\
		static const char * GLenumStr()	{ return #GLENUM; }						\
		static const char * EnumStr()	{ return #ENUM_TYPE ##"::"#ENUM_VAL; }	\
		static const GLenum	glEnum = GLENUM;									\
		static const u32 major = MAJOR;											\
		static const u32 minor = MINOR;											\
	};

namespace djah { namespace opengl {

	struct enum_descriptor
	{
		const GLenum glEnum;
		const u32	 glEnumHash;
		const u32	 enumHash;
		const char * glEnumStr;
		const char * enumStr;
		const u32    majorVersion:4;
		const u32    minorVersion:4;
	};

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_GL_ENUM_HPP */