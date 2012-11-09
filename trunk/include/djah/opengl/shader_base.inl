namespace djah { namespace opengl {

	//----------------------------------------------------------------------------------------------
	template<int ShaderType>
	shader_base<ShaderType>::shader_base(const std::string &source, const std::string &_fileName)
		: fileName_(fileName_)
	{
		aquire();
		if( !source.empty() )
			loadSource(source);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int ShaderType>
	shader_base<ShaderType>::~shader_base()
	{
		release();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int ShaderType>
	inline void shader_base<ShaderType>::aquire()
	{
		id_ = glCreateShader(ShaderType);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int ShaderType>
	inline void shader_base<ShaderType>::release()
	{
		glDeleteShader(id_);
		id_ = INVALID_ID;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int ShaderType>
	inline bool shader_base<ShaderType>::isValidResource() const
	{
		return glIsShader(id_) != 0;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int ShaderType>
	inline const std::string& shader_base<ShaderType>::fileName() const
	{
		return fileName_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int ShaderType>
	void shader_base<ShaderType>::loadSource(const std::string &source)
	{
		source_ = source;
		loadSource();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int ShaderType>
	inline void shader_base<ShaderType>::loadSource() const
	{
		const char *src_ptr = source_.c_str();
		glShaderSource(id_, 1, &src_ptr, 0);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int ShaderType>
	inline bool shader_base<ShaderType>::compile() const
	{
		glCompileShader(id_);
		return handleCompilationErrors();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int ShaderType>
	bool shader_base<ShaderType>::handleCompilationErrors() const
	{
		// First of all check if the compilation went well
		int status = GL_TRUE;
		glGetShaderiv(id_, GL_COMPILE_STATUS, &status);
		if( status != GL_TRUE )
		{
			// Retrieve log size
			int log_size = 0;
			glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &log_size);

			// Allocate a string big enough to contain the log + '\0'
			std::unique_ptr<char[]> log_str(new char[log_size + 1]);

			// Retrieve the log
			glGetShaderInfoLog(id_, log_size, &log_size, log_str.get());
			log_str[log_size] = '\0';

			// TODO : throw some exception
			DJAH_OPENGL_CRITICAL()
				<< "====================================================================\n"
				<< "Errors in " << fileName_ << "\n"
				<< "--------------------------------------------------------------------\n"
				<< log_str.get()
				<< "===================================================================="
				<< DJAH_END_LOG();
		}
		return (status == GL_TRUE);
	}
	//----------------------------------------------------------------------------------------------

} /*opengl*/ } /*djah*/