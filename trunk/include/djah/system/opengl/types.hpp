#ifndef DJAH_SYSTEM_OPENGL_TYPES_HPP
#define DJAH_SYSTEM_OPENGL_TYPES_HPP

#include "gl.hpp"

namespace djah { namespace system { namespace opengl {

	//----------------------------------------------------------------------------------------------
	template<typename T>
	struct type;
	//----------------------------------------------------------------------------------------------
	template<>
	struct type<char>			{ enum { Type = GL_BYTE };				};
	//----------------------------------------------------------------------------------------------
	template<>
	struct type<unsigned char>	{ enum { Type = GL_UNSIGNED_BYTE };		};
	//----------------------------------------------------------------------------------------------
	template<>
	struct type<short>			{ enum { Type = GL_SHORT };				};
	//----------------------------------------------------------------------------------------------
	template<>
	struct type<unsigned short>	{ enum { Type = GL_UNSIGNED_SHORT };	};
	//----------------------------------------------------------------------------------------------
	template<>
	struct type<int>			{ enum { Type = GL_INT };				};
	//----------------------------------------------------------------------------------------------
	template<>
	struct type<unsigned int>	{ enum { Type = GL_UNSIGNED_INT };		};
	//----------------------------------------------------------------------------------------------
	template<>
	struct type<float>			{ enum { Type = GL_FLOAT };				};
	//----------------------------------------------------------------------------------------------
	#ifdef GL_DOUBLE
	template<>
	struct type<double>			{ enum { Type = GL_DOUBLE };			};
	#endif
	//----------------------------------------------------------------------------------------------

} /*opengl*/ } /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_OPENGL_TYPES_HPP */