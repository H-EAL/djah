namespace djah { namespace video { namespace drivers { namespace ogl {

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
	namespace priv {

		template<typename T>
		struct UniformFunc;

		template<>
		struct UniformFunc<float>
		{
			static PFNGLUNIFORM1FVPROC uniform(size_t i) 
			{
				static const PFNGLUNIFORM1FVPROC uniformTab[] =
				{ glUniform1fv, glUniform2fv, glUniform3fv, glUniform4fv };
				return uniformTab[i];
			}
		};

		template<>
		struct UniformFunc<int>
		{
			static PFNGLUNIFORM1IVPROC uniform(size_t i) 
			{
				static const PFNGLUNIFORM1IVPROC uniformTab[] =
				{ glUniform1iv, glUniform2iv, glUniform3iv, glUniform4iv };
				return uniformTab[i];
			}
		};

	} /*priv*/
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	void shader::sendUniform(const std::string &name, const T *data, int count) const
	{
		unsigned int location = getUniformLocation(name);
		priv::UniformFunc<T>::uniform(SIZE-1)(location, count, data);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<size_t SIZE>
	void shader::sendUniformMatrix(const std::string &name, const float *data, int count, bool transpose) const
	{
		static const PFNGLUNIFORMMATRIX2FVPROC uniformMatrixFuncTab[] = 
		{
			glUniformMatrix2fv,
			glUniformMatrix3fv,
			glUniformMatrix4fv
		};

		static const PFNGLUNIFORMMATRIX2FVPROC uniformMatrix = uniformMatrixFuncTab[SIZE-2];

		unsigned int location = getUniformLocation(name);
		uniformMatrix(location, count, transpose, data);
	}
	//----------------------------------------------------------------------------------------------
	template<size_t SIZE>
	void shader::sendUniformMatrix(const std::string &name, const math::basic_matrix<SIZE,float> &mat, bool transpose) const
	{
		sendUniformMatrix<SIZE>(name, mat.data(), 1, transpose);
	}
	//----------------------------------------------------------------------------------------------

} /*ogl*/ } /*drivers*/ } /*video*/ } /*djah*/