#include "djah/opengl/shader_program.hpp"
#include "djah/opengl/errors.hpp"
#include "djah/debug/log.hpp"
#include "djah/debug/assertion.hpp"


namespace djah { namespace opengl {

	//----------------------------------------------------------------------------------------------
	shader_program::shader_program(const std::string &shaderName)
		: resource(shaderName)
	{
		aquire();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	shader_program::~shader_program()
	{
		release();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void shader_program::aquire()
	{
		id_ = glCreateProgram();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void shader_program::release()
	{
		glDeleteProgram(id_);
		id_ = INVALID_ID;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool shader_program::isValidResource() const
	{
		return glIsProgram(id_) != 0;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool shader_program::link()
	{
		uniformCache_.clear();

		glLinkProgram(id_);
		const bool success = handleLinkingErrors();

		if( success )
		{
			initializeUniformMap();
			initializeAttributeMap();
		}

		return success;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void shader_program::begin() const
	{
		glUseProgram(id_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void shader_program::end()
	{
		glUseProgram(0);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	int shader_program::getVertexAttributeLocation(const std::string &name) const
	{
		unsigned int location = -1;
		cache_t::const_iterator it = attributeCache_.find(name);
		
		if( it != attributeCache_.end() )
		{
			location = it->second;
		}
		else
		{
			DJAH_OPENGL_WARNING()
				<< "Unable to find attribute \"" << name << "\" in [" << type_name() << "]" << name_
				<< DJAH_END_LOG();
		}

		return location;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool shader_program::handleLinkingErrors() const
	{
		// First of all check if the linking went well
		int status = GL_TRUE;
		glGetProgramiv(id_, GL_LINK_STATUS, &status);
		if( status != GL_TRUE )
		{
			// Retrieve log size
			int log_size = 0;
			glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &log_size);

			// Allocate a string big enough to contain the log + '\0'
			std::unique_ptr<char[]> log_str(new char[log_size + 1]);

			// Retrieve the log
			glGetProgramInfoLog(id_, log_size, &log_size, log_str.get());
			log_str[log_size] = '\0';

			// TODO : let the error policy handle this
			DJAH_ASSERT_MSG( false, "Errors in %s\n%s",
				name_.empty() ? "(unnamed)" : name_.c_str(), log_str.get() );
		}
		return (status == GL_TRUE);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	int shader_program::getUniformLocation(const std::string &name) const
	{
		cache_t::const_iterator it = uniformCache_.find(name);

		DJAH_ASSERT_MSG( it != uniformCache_.end(), "Unable to find uniform \"%s\" in [%s]%s"
			, name.c_str(), type_name(), name_.c_str() );

		return (it != uniformCache_.end()) ? it->second : -1;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void shader_program::initializeUniformMap()
	{
		int nbActiveUniforms = 0;
		glGetProgramiv(id_, GL_ACTIVE_UNIFORMS, &nbActiveUniforms);

		for( int i = 0; i < nbActiveUniforms; ++i )
		{
			int length = 0;
			int size = 0;
			GLenum type = GL_NONE;
			char name[1024];
			glGetActiveUniform(id_, i, 1024, &length, &size, &type, name);

			uniformCache_[name] = glGetUniformLocation(id_, name);
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void shader_program::initializeAttributeMap()
	{
		int nbActiveAttributes = 0;
		glGetProgramiv(id_, GL_ACTIVE_ATTRIBUTES, &nbActiveAttributes);

		for( int i = 0; i < nbActiveAttributes; ++i )
		{
			int length = 0;
			int size = 0;
			GLenum type = GL_NONE;
			char name[1024];
			glGetActiveAttrib(id_, i, 1024, &length, &size, &type, name);

			attributeCache_[name] = glGetAttribLocation(id_, name);
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void shader_program::sendUniform(const std::string &name, float u1) const
	{
		int location = getUniformLocation(name);
		glUniform1f(location, u1);
	}
	//----------------------------------------------------------------------------------------------
	void shader_program::sendUniform(const std::string &name, float u1, float u2) const
	{
		int location = getUniformLocation(name);
		glUniform2f(location, u1, u2);
	}
	//----------------------------------------------------------------------------------------------
	void shader_program::sendUniform(const std::string &name, float u1, float u2, float u3) const
	{
		int location = getUniformLocation(name);
		glUniform3f(location, u1, u2, u3);
	}
	//----------------------------------------------------------------------------------------------
	void shader_program::sendUniform(const std::string &name, float u1, float u2, float u3, float u4) const
	{
		int location = getUniformLocation(name);
		glUniform4f(location, u1, u2, u3, u4);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void shader_program::sendUniform(const std::string &name, int u1) const
	{
		int location = getUniformLocation(name);
		glUniform1i(location, u1);
	}
	//----------------------------------------------------------------------------------------------
	void shader_program::sendUniform(const std::string &name, int u1, int u2) const
	{
		int location = getUniformLocation(name);
		glUniform2i(location, u1, u2);
	}
	//----------------------------------------------------------------------------------------------
	void shader_program::sendUniform(const std::string &name, int u1, int u2, int u3) const
	{
		int location = getUniformLocation(name);
		glUniform3i(location, u1, u2, u3);
	}
	//----------------------------------------------------------------------------------------------
	void shader_program::sendUniform(const std::string &name, int u1, int u2, int u3, int u4) const
	{
		int location = getUniformLocation(name);
		glUniform4i(location, u1, u2, u3, u4);
	}
	//----------------------------------------------------------------------------------------------

} /*opengl*/ } /*djah*/