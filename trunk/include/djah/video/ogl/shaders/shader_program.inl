namespace djah { namespace video { namespace ogl {

	//----------------------------------------------------------------------------------------------
	namespace priv {

		template<typename T>
		struct UniformFunc;

		template<>
		struct UniformFunc<float>
		{
			//typedef void (*PFNGLUNIFORMPROC) (GLint, GLsizei, const GLfloat*);
			static PFNGLUNIFORM1FVPROC uniform(int i) 
			{
				static const PFNGLUNIFORM1FVPROC uniformTab[] =
				{
					glUniform1fv, 
					glUniform2fv,
					glUniform3fv,
					glUniform4fv
				};
				return uniformTab[i];
			}
		};

		template<>
		struct UniformFunc<int>
		{
			typedef void (*PFNGLUNIFORMPROC) (GLint, GLsizei, const GLint*);
			static PFNGLUNIFORMPROC uniform(int i) 
			{
				static const PFNGLUNIFORMPROC uniformTab[] =
				{ 
					(PFNGLUNIFORMPROC) glUniform1iv, 
					(PFNGLUNIFORMPROC) glUniform2iv, 
					(PFNGLUNIFORMPROC) glUniform3iv, 
					(PFNGLUNIFORMPROC) glUniform4iv
				};
				return uniformTab[i];
			}
		};

	} /*priv*/
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	void shader_program::sendUniform(const std::string &name, const T *data, int count) const
	{
		unsigned int location = getUniformLocation(name);
		priv::UniformFunc<T>::uniform(N-1)(location, count, data);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int ShaderType>
	void shader_program::attach(const shader_base<ShaderType> &s) const
	{
		glAttachShader(id_, s.id());
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int ShaderType>
	void shader_program::detach(const shader_base<ShaderType> &s) const
	{
		glDetachShader(id_, s.id());
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int N>
	void shader_program::sendUniformMatrix(const std::string &name, const float *data, int count, bool transpose) const
	{
		//typedef void (*PFNGLUNIFORMMATRIXPROC) (GLint, GLsizei, GLboolean, const GLfloat*); 
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
	void shader_program::sendUniformMatrix(const std::string &name, const math::matrix<N,float> &mat, bool transpose) const
	{
		sendUniformMatrix<N>(name, mat.data, 1, transpose);
	}
	//----------------------------------------------------------------------------------------------

} /*ogl*/ } /*video*/ } /*djah*/