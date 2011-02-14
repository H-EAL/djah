#include <GL/glew.h>
#include <boost/smart_ptr/scoped_array.hpp>

#include "video/drivers/ogl/shaders/shader.hpp"
#include "video/drivers/ogl/errors.hpp"
#include "log/logger.hpp"


namespace djah { namespace video { namespace drivers { namespace ogl {

	//----------------------------------------------------------------------------------------------
	shader::shader()
	{
		aquire();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	shader::~shader()
	{
		release();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void shader::aquire()
	{
		DJAH_TEST_FOR_OPENGL_ERRORS
		(
			id_ = glCreateProgram()
		);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void shader::release()
	{
		glDeleteProgram(id_);
		id_ = INVALID_ID;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int ShaderType>
	void shader::attach(const shader_base<ShaderType> &s) const
	{
		glAttachShader(id_, s.id());
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int ShaderType>
	void shader::detach(const shader_base<ShaderType> &s) const
	{
		glDetachShader(id_, s.id());
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void shader::link() const
	{
		cache_.clear();
		DJAH_TEST_FOR_OPENGL_ERRORS
		(
			glLinkProgram(id_)
		);
		handleLinkingErrors();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void shader::begin() const
	{
		glUseProgram(id_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void shader::end()
	{
		glUseProgram(0);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	unsigned int shader::getVertexAttributeLocation(const std::string &name) const
	{
		return glGetAttribLocation(id_, name.c_str());
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void shader::handleLinkingErrors() const
	{
		// First of all check if the linking went well
		int status = GL_TRUE;
		glGetProgramiv(id_, GL_LINK_STATUS, &status);
		if( status == GL_TRUE )
			return; // No error

		// Retrieve log size
		int log_size = 0;
		glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &log_size);

		// Allocate a string big enough to contain the log + '\0'
		boost::scoped_array<char> log_str(new char[log_size + 1]);

		// Retrieve the log
		glGetProgramInfoLog(id_, log_size, &log_size, log_str.get());

		// TODO : let the error policy handle this
		log::logger::log(log::EWL_CRITICAL)
			<< "====================================================================\n"
			<< "                      SHADER LINKING ERROR(S)                       \n"
			<< "--------------------------------------------------------------------\n"
			<< log_str.get()
			<< "===================================================================="
			<< log::logger::endl();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	unsigned int shader::getUniformLocation(const std::string &name) const
	{
		// First search for the uniform in cache
		uniform_cache_t::const_iterator it = cache_.find(name);
		// If not found get it and add it to cache
		if( it == cache_.end() )
			cache_[name] = glGetUniformLocation(id_, name.c_str());

		return cache_[name];
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void shader::sendUniform(const std::string &name, float u1) const
	{
		unsigned int location = getUniformLocation(name);
		glUniform1f(location, u1);
	}
	//----------------------------------------------------------------------------------------------
	void shader::sendUniform(const std::string &name, float u1, float u2) const
	{
		unsigned int location = getUniformLocation(name);
		glUniform2f(location, u1, u2);
	}
	//----------------------------------------------------------------------------------------------
	void shader::sendUniform(const std::string &name, float u1, float u2, float u3) const
	{
		unsigned int location = getUniformLocation(name);
		glUniform3f(location, u1, u2, u3);
	}
	//----------------------------------------------------------------------------------------------
	void shader::sendUniform(const std::string &name, float u1, float u2, float u3, float u4) const
	{
		unsigned int location = getUniformLocation(name);
		glUniform4f(location, u1, u2, u3, u4);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void shader::sendUniform(const std::string &name, int u1) const
	{
		unsigned int location = getUniformLocation(name);
		glUniform1i(location, u1);
	}
	//----------------------------------------------------------------------------------------------
	void shader::sendUniform(const std::string &name, int u1, int u2) const
	{
		unsigned int location = getUniformLocation(name);
		glUniform2i(location, u1, u2);
	}
	//----------------------------------------------------------------------------------------------
	void shader::sendUniform(const std::string &name, int u1, int u2, int u3) const
	{
		unsigned int location = getUniformLocation(name);
		glUniform3i(location, u1, u2, u3);
	}
	//----------------------------------------------------------------------------------------------
	void shader::sendUniform(const std::string &name, int u1, int u2, int u3, int u4) const
	{
		unsigned int location = getUniformLocation(name);
		glUniform4i(location, u1, u2, u3, u4);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int N>
	void shader::sendUniformMatrix(const std::string &name, const float *data, int count, bool transpose) const
	{
		static const PFNGLUNIFORMMATRIX2FVPROC uniformMatrixFuncTab[] = 
		{
			glUniformMatrix2fv,
			glUniformMatrix3fv,
			glUniformMatrix4fv
		};

		static const PFNGLUNIFORMMATRIX2FVPROC uniformMatrix = uniformMatrixFuncTab[N-2];

		unsigned int location = getUniformLocation(name);
		uniformMatrix(location, count, transpose, data);
	}
	//----------------------------------------------------------------------------------------------
	template<int N>
	void shader::sendUniformMatrix(const std::string &name, const math::matrix<N,float> &mat, bool transpose) const
	{
		sendUniformMatrix<N>(name, mat.data, 1, transpose);
	}
	//----------------------------------------------------------------------------------------------

} /*ogl*/ } /*drivers*/ } /*video*/ } /*djah*/