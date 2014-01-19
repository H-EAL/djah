namespace djah { namespace opengl {

	//----------------------------------------------------------------------------------------------
	namespace priv {

		//------------------------------------------------------------------------------------------
		template<typename T>
		struct uniform_func;
		//------------------------------------------------------------------------------------------
		template<>
		struct uniform_func<float>
		{
			//typedef void (*PFNGLUNIFORMPROC) (GLint, GLsizei, const GLfloat*);
			template<int N>
			static PFNGLUNIFORM1FVPROC uniform() 
			{
				//DJAH_STATIC_ASSERT(N >= 0 && N < 4, "Out of bounds");
				static const PFNGLUNIFORM1FVPROC uniformTab[] =
				{
					glUniform1fv, 
					glUniform2fv,
					glUniform3fv,
					glUniform4fv
				};
				return uniformTab[N];
			}
		};
		//------------------------------------------------------------------------------------------
		template<>
		struct uniform_func<int>
		{
			//typedef void (*PFNGLUNIFORMPROC) (GLint, GLsizei, const GLint*);
			template<int N>
			static PFNGLUNIFORM1IVPROC uniform() 
			{
				DJAH_STATIC_ASSERT(N >= 0 && N < 4, "Out of bounds");
				static const PFNGLUNIFORM1IVPROC uniformTab[] =
				{ 
					glUniform1iv, 
					glUniform2iv, 
					glUniform3iv, 
					glUniform4iv
				};
				return uniformTab[N];
			}
		};
		//------------------------------------------------------------------------------------------
		template<>
		struct uniform_func<unsigned int>
		{
			//typedef void (*PFNGLUNIFORMPROC) (GLint, GLsizei, const GLint*);
			template<int N>
			static PFNGLUNIFORM1IVPROC uniform() 
			{
				DJAH_STATIC_ASSERT(N >= 0 && N < 4, "Out of bounds");
				static const PFNGLUNIFORM1UIVPROC uniformTab[] =
				{ 
					glUniform1uiv, 
					glUniform2uiv, 
					glUniform3uiv, 
					glUniform4uiv
				};
				return uniformTab[N];
			}
		};
		//------------------------------------------------------------------------------------------


		//------------------------------------------------------------------------------------------
		template<typename T>
		struct uniform_getter;
		//------------------------------------------------------------------------------------------
		template<>
		struct uniform_getter<float>
		{
			static void value_of(unsigned int id, int location, float &val)
			{
				glGetUniformfv(id, location, &val);
			}
		};
		//------------------------------------------------------------------------------------------
		template<>
		struct uniform_getter<double>
		{
			static void value_of(unsigned int id, int location, double &val)
			{
				glGetUniformdv(id, location, &val);
			}
		};
		//------------------------------------------------------------------------------------------
		template<>
		struct uniform_getter<int>
		{
			static void value_of(unsigned int id, int location, int &val)
			{
				glGetUniformiv(id, location, &val);
			}
		};
		//------------------------------------------------------------------------------------------
		template<>
		struct uniform_getter<unsigned int>
		{
			static void value_of(unsigned int id, int location, unsigned int &val)
			{
				glGetUniformuiv(id, location, &val);
			}
		};
		//------------------------------------------------------------------------------------------
		template<>
		struct uniform_getter<bool>
		{
			static void value_of(unsigned int id, int location, bool &val)
			{
				int v = 0;
				glGetUniformiv(id, location, &v);
				val = (v != 0);
			}
		};
		//------------------------------------------------------------------------------------------
		template<int N, typename T>
		struct uniform_getter<math::vector<N,T>>
		{
			static void value_of(unsigned int id, int location, math::vector<N,T> &val)
			{
				uniform_getter<T>::value_of(id, location, val[0]);
			}
		};
		//------------------------------------------------------------------------------------------
		template<typename T>
		struct uniform_getter<math::quaternion<T>>
		{
			static void value_of(unsigned int id, int location, math::quaternion<T> &val)
			{
				uniform_getter<T>::value_of(id, location, val[0]);
			}
		};
		//------------------------------------------------------------------------------------------
		template<int N, int M, typename T>
		struct uniform_getter<math::matrix<N,M,T>>
		{
			static void value_of(unsigned int id, int location, math::matrix<N,M,T> &val)
			{
				uniform_getter<T>::value_of(id, location, val[0][0]);
			}
		};
		//------------------------------------------------------------------------------------------

	} /*priv*/
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
	template<typename T>
	T shader_program::getUniform(const std::string &name) const
	{
		T uniformValue;

		const int location = getUniformLocation(name);
		if( ensure(location != -1) )
		{
			priv::uniform_getter<T>::value_of(id_, location, uniformValue);
		}

		return uniformValue;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	void shader_program::sendUniform(const std::string &name, T val) const
	{
		const int location = getUniformLocation(name);
		priv::uniform_func<T>::uniform<0>()(location, 1, &val);
	}
	//----------------------------------------------------------------------------------------------
	template<typename T, int S>
	void shader_program::sendUniform(const std::string &name, const T (&array)[S]) const
	{
		const int location = getUniformLocation(name);
		priv::uniform_func<T>::uniform<0>()(location, S, &array[0]);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	void shader_program::sendUniform(const std::string &name, const T *data, int count) const
	{
		const int location = getUniformLocation(name);
		priv::uniform_func<T>::uniform<N-1>()(location, count, data);
	}
	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	void shader_program::sendUniform(const std::string &name, const math::vector<N,T> &vec) const
	{
		sendUniform<N>(name, vec.data, 1);
	}
	//----------------------------------------------------------------------------------------------
	template<int N, typename T, int S>
	void shader_program::sendUniform(const std::string &name, const math::vector<N,T> (&vecArray)[S]) const
	{
		sendUniform<N>(name, vec[0].data, S);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int M, int N>
	void shader_program::sendUniform(const std::string &name, const float *data, int count, bool transpose) const
	{
		//typedef void (*PFNGLUNIFORMMATRIXPROC) (GLint, GLsizei, GLboolean, const GLfloat*); 
		static const PFNGLUNIFORMMATRIX2FVPROC uniformMatrixFuncTab[3][3] = 
		{
			{ glUniformMatrix2fv,	glUniformMatrix2x3fv,	glUniformMatrix2x4fv	},
			{ glUniformMatrix3x2fv,	glUniformMatrix3fv,		glUniformMatrix3x4fv	},
			{ glUniformMatrix4x2fv, glUniformMatrix4x3fv,	glUniformMatrix4fv		}
		};

		static const PFNGLUNIFORMMATRIX2FVPROC uniformMatrix = uniformMatrixFuncTab[M-2][N-2];

		const int location = getUniformLocation(name);
		if( ensure(location != -1) )
		{
			uniformMatrix(location, count, transpose, data);
		}
	}
	//----------------------------------------------------------------------------------------------
	template<int M, int N>
	void shader_program::sendUniform(const std::string &name, const math::matrix<M,N,float> &mat, bool transpose) const
	{
		sendUniform<M,N>(name, &mat.data[0][0], 1, transpose);
	}
	//----------------------------------------------------------------------------------------------
	template<int M, int N, int S>
	void shader_program::sendUniform(const std::string &name, const math::matrix<M,N,float> (&matArray)[S], bool transpose) const
	{
		sendUniform<M,N>(name, &mat[0].data[0][0], S, transpose);
	}
	//----------------------------------------------------------------------------------------------

} /*opengl*/ } /*djah*/
