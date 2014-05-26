#ifndef DJAH_OPENGL_ENUM_BUFFER_TARGET_HPP
#define DJAH_OPENGL_ENUM_BUFFER_TARGET_HPP

#include "djah/system/gl.hpp"
#include "djah/opengl/gl_enum.hpp"

namespace djah { namespace opengl {

	namespace BufferTarget
	{
		//////////////////////////////////////////////////////////////////////////
		// Actual C++ enum
		#define GLENUM_PROCESSOR DJAH_BUILD_ENUM
		enum Type
		{
			#include "enum_buffer_target.inl"
			BufferTarget_Count
		};
		#undef GLENUM_PROCESSOR

		//////////////////////////////////////////////////////////////////////////
		// Compile time lookup
		#define GLENUM_PROCESSOR DJAH_BUILD_GL_ENUM_COMPILETIME_LOOK_UP
		template<BufferTarget::Type> struct ToGL;
		#include "enum_buffer_target.inl"
		#undef GLENUM_PROCESSOR

		//////////////////////////////////////////////////////////////////////////
		// Runtime lookup
		#define GLENUM_PROCESSOR DJAH_BUILD_GL_ENUM_RUNTIME_LOOK_UP
		static const enum_descriptor Descriptors[BufferTarget_Count] = 
		{
			#include "enum_buffer_target.inl"
		};
		#undef GLENUM_PROCESSOR

	} /*BufferTarget*/

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_ENUM_BUFFER_TARGET_HPP */