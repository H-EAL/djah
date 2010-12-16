namespace djah { namespace video { namespace drivers { namespace ogl {

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

} /*ogl*/ } /*drivers*/ } /*video*/ } /*djah*/