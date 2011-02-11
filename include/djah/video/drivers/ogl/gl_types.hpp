#ifndef DJAH_VIDEO_DRIVERS_OGL_GL_TYPES_HPP
#define DJAH_VIDEO_DRIVERS_OGL_GL_TYPES_HPP

namespace djah { namespace video { namespace drivers { namespace ogl {

	//----------------------------------------------------------------------------------------------
	template<typename T>
	struct gl_type;
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
	template<>
	struct gl_type<double>			{ enum { Type = GL_DOUBLE };			};
	//----------------------------------------------------------------------------------------------

} /*ogl*/ } /*drivers*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_DRIVERS_OGL_GL_TYPES_HPP */