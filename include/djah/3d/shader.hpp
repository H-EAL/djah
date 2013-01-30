#ifndef DJAH_3D_SHADER_HPP
#define DJAH_3D_SHADER_HPP

#include <string>
#include "djah/opengl/vertex_shader.hpp"
#include "djah/opengl/tessellation_shader.hpp"
#include "djah/opengl/geometry_shader.hpp"
#include "djah/opengl/pixel_shader.hpp"
#include "djah/opengl/shader_program.hpp"

namespace djah { namespace d3d {

	class shader
	{
	public:
		shader(const std::string &shaderName);
		opengl::shader_program& program() { return shaderProgram_; }

	private:
		std::string loadSource(const std::string &fileName) const;

	private:
		opengl::vertex_shader					vertexShader_;
		opengl::tessellation_control_shader		tessCtrlShader_;
		opengl::tessellation_evaluation_shader	tessEvalShader_;
		opengl::geometry_shader					geometryShader_;
		opengl::fragment_shader					fragmentShader_;
		opengl::shader_program					shaderProgram_;
	};

} /*d3d*/ } /*djah*/

#endif /* DJAH_3D_SHADER_HPP */