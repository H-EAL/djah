#ifndef DJAH_VIDEO_OGL_ERRORS_HPP
#define DJAH_VIDEO_OGL_ERRORS_HPP

#include "../../debug/logger.hpp"

//--------------------------------------------------------------------------------------------------
#define DJAH_TEST_FOR_OPENGL_ERRORS(func)	\
	func;									\
	djah::video::ogl::handle_error(glGetError(), #func, __FILE__, __FUNCTION__, __LINE__);
//--------------------------------------------------------------------------------------------------


namespace djah { namespace video { namespace ogl {

	//----------------------------------------------------------------------------------------------
	inline const char* get_error_str(GLenum error)
	{
		#define OGL_ADD_ERROR_STRING(error)	case error: return #error;
		switch(error)
		{
			OGL_ADD_ERROR_STRING(GL_INVALID_ENUM)
			OGL_ADD_ERROR_STRING(GL_INVALID_VALUE)
			OGL_ADD_ERROR_STRING(GL_INVALID_OPERATION)
			//OGL_ADD_ERROR_STRING(GL_STACK_OVERFLOW)
			//OGL_ADD_ERROR_STRING(GL_STACK_UNDERFLOW)
			OGL_ADD_ERROR_STRING(GL_OUT_OF_MEMORY)
		}
		#undef OGL_ADD_ERROR_STRING
		return "Unhandled error";
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	inline void handle_error(GLenum error, const char *call, const char *file, const char *function, long line)
	{
		if(error == GL_NO_ERROR)
			return;

		debug::logger::log(debug::EWL_CRITICAL)	<< get_error_str(error)	<< " "
											<< call					<< " "
											<< file					<< " "
											<< function				<< " "
											<< line-1
											<< debug::logger::endl();
	}
	//----------------------------------------------------------------------------------------------

} /*ogl*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_OGL_ERRORS_HPP */
