#ifndef DJAH_OPENGL_TESSELLATION_SHADER_HPP
#define DJAH_OPENGL_TESSELLATION_SHADER_HPP

#include "shader_base.hpp"

namespace djah { namespace opengl {

	typedef shader_base<GL_TESS_CONTROL_SHADER>    tessellation_control_shader;
	typedef shader_base<GL_TESS_EVALUATION_SHADER> tessellation_evaluation_shader;

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_TESSELATION_SHADER_HPP */
