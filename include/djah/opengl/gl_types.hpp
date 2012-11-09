#ifndef DJAH_OPENGL_GL_TYPES_HPP
#define DJAH_OPENGL_GL_TYPES_HPP

#include "system/gl.hpp"

namespace djah { namespace opengl {

	//----------------------------------------------------------------------------------------------
	template<typename T>
	struct gl_type					{ enum { Type = GL_NONE };				};
	//----------------------------------------------------------------------------------------------
	template<>
	struct gl_type<char>			{ enum { Type = GL_BYTE };				};
	//----------------------------------------------------------------------------------------------
	template<>
	struct gl_type<unsigned char>	{ enum { Type = GL_UNSIGNED_BYTE };		};
	//----------------------------------------------------------------------------------------------
	template<>
	struct gl_type<short>			{ enum { Type = GL_SHORT };				};
	//----------------------------------------------------------------------------------------------
	template<>
	struct gl_type<unsigned short>	{ enum { Type = GL_UNSIGNED_SHORT };	};
	//----------------------------------------------------------------------------------------------
	template<>
	struct gl_type<int>				{ enum { Type = GL_INT };				};
	//----------------------------------------------------------------------------------------------
	template<>
	struct gl_type<unsigned int>	{ enum { Type = GL_UNSIGNED_INT };		};
	//----------------------------------------------------------------------------------------------
	template<>
	struct gl_type<float>			{ enum { Type = GL_FLOAT };				};
	//----------------------------------------------------------------------------------------------
	#ifdef GL_DOUBLE
	template<>
	struct gl_type<double>			{ enum { Type = GL_DOUBLE };			};
	#endif
	//----------------------------------------------------------------------------------------------

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_GL_TYPES_HPP */