#ifndef DJAH_OPENGL_ENUM_BUFFER_USAGE_HPP
#define DJAH_OPENGL_ENUM_BUFFER_USAGE_HPP

#include "djah/system/gl.hpp"
#include "djah/opengl/gl_enum.hpp"

namespace djah { namespace opengl {

	namespace BufferUsage
	{
		//////////////////////////////////////////////////////////////////////////
		// Actual C++ enum
		#define GLENUM_PROCESSOR DJAH_BUILD_ENUM
		enum Type
		{
			#include "enum_buffer_usage.inl"
			BufferUsage_Count
		};
		#undef GLENUM_PROCESSOR

		//////////////////////////////////////////////////////////////////////////
		// Compile time lookup
		#define GLENUM_PROCESSOR DJAH_BUILD_GL_ENUM_COMPILETIME_LOOK_UP
		template<BufferUsage::Type> struct ToGL;
		#include "enum_buffer_usage.inl"
		#undef GLENUM_PROCESSOR

		//////////////////////////////////////////////////////////////////////////
		// Runtime lookup
		#define GLENUM_PROCESSOR DJAH_BUILD_GL_ENUM_RUNTIME_LOOK_UP
		static const enum_descriptor Descriptors[BufferUsage_Count] = 
		{
			#include "enum_buffer_usage.inl"
		};
		#undef GLENUM_PROCESSOR

	} /*BufferUsage*/

	namespace BufferAccessFrequency
	{
		enum Type
		{
			Stream,
			Static,
			Dynamic,

			BufferAccessFrequency_Count
		};
	} /*BufferAccessFrequency*/

	namespace BufferAccessType
	{
		enum Type
		{
			Draw,
			Read,
			Copy,

			BufferAccessType_Count
		};
	} /*BufferAccessType*/

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_ENUM_BUFFER_USAGE_HPP */