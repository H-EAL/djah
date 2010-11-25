#ifndef DJAH_VIDEO_DRIVERS_OGL_GL_TYPES_HPP
#define DJAH_VIDEO_DRIVERS_OGL_GL_TYPES_HPP

namespace djah { namespace video { namespace drivers { namespace ogl {

	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline unsigned int get_gl_type();
	//----------------------------------------------------------------------------------------------
	template<>
	inline unsigned int get_gl_type<char>()           { return GL_BYTE; }
	//----------------------------------------------------------------------------------------------
	template<>
	inline unsigned int get_gl_type<unsigned char>()  { return GL_UNSIGNED_BYTE; }
	//----------------------------------------------------------------------------------------------
	template<>
	inline unsigned int get_gl_type<short>()          { return GL_SHORT; }
	//----------------------------------------------------------------------------------------------
	template<>
	inline unsigned int get_gl_type<unsigned short>() { return GL_UNSIGNED_SHORT; }
	//----------------------------------------------------------------------------------------------
	template<>
	inline unsigned int get_gl_type<int>()            { return GL_INT; }
	//----------------------------------------------------------------------------------------------
	template<>
	inline unsigned int get_gl_type<unsigned int>()   { return GL_UNSIGNED_INT; }
	//----------------------------------------------------------------------------------------------
	template<>
	inline unsigned int get_gl_type<float>()          { return GL_FLOAT; }
	//----------------------------------------------------------------------------------------------
	template<>
	inline unsigned int get_gl_type<double>()         { return GL_DOUBLE; }
	//----------------------------------------------------------------------------------------------

} /*ogl*/ } /*drivers*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_DRIVERS_OGL_GL_TYPES_HPP */