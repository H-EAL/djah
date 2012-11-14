namespace djah { namespace opengl {

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
		int location = getUniformLocation(name);
		priv::UniformFunc<T>::uniform(N-1)(location, count, data);
	}
	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	void shader_program::sendUniform(const std::string &name, const math::vector<N,T> &vec) const
	{
		sendUniform<N>(name, vec.data, 1);
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
	template<int M, int N>
	void shader_program::sendUniformMatrix(const std::string &name, const float *data, int count, bool transpose) const
	{
		//typedef void (*PFNGLUNIFORMMATRIXPROC) (GLint, GLsizei, GLboolean, const GLfloat*); 
		static const PFNGLUNIFORMMATRIX2FVPROC uniformMatrixFuncTab[3][3] = 
		{
			{ glUniformMatrix2fv,	glUniformMatrix2x3fv,	glUniformMatrix2x4fv	},
			{ glUniformMatrix3x2fv,	glUniformMatrix3fv,		glUniformMatrix3x4fv	},
			{ glUniformMatrix4x2fv, glUniformMatrix4x3fv,	glUniformMatrix4fv		}
		};

		static const PFNGLUNIFORMMATRIX2FVPROC uniformMatrix = uniformMatrixFuncTab[M-2][N-2];

		int location = getUniformLocation(name);
		uniformMatrix(location, count, transpose, data);
	}
	//----------------------------------------------------------------------------------------------
	template<int M, int N>
	void shader_program::sendUniformMatrix(const std::string &name, const math::matrix<M,N,float> &mat, bool transpose) const
	{
		sendUniformMatrix<M,N>(name, &mat.data[0][0], 1, transpose);
	}
	//----------------------------------------------------------------------------------------------

} /*opengl*/ } /*djah*/
