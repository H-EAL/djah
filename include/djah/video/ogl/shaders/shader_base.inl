namespace djah { namespace video { namespace ogl {

	//----------------------------------------------------------------------------------------------
	template<int ShaderType>
	shader_base<ShaderType>::shader_base(const std::string &url)
	{
		aquire();
		if( !url.empty() )
			loadSourceFromUrl(url);
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
	void shader_base<ShaderType>::aquire()
	{
		id_ = glCreateShader(ShaderType);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int ShaderType>
	void shader_base<ShaderType>::release()
	{
		glDeleteShader(id_);
		id_ = INVALID_ID;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int ShaderType>
	bool shader_base<ShaderType>::isValidResource() const
	{
		return glIsShader(id_) != 0;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int ShaderType>
	void shader_base<ShaderType>::loadSourceFromString(const std::string &source)
	{
		source_ = source;
		loadSource();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int ShaderType>
	void shader_base<ShaderType>::loadSourceFromUrl(const std::string &url)
	{
		filesystem::stream_ptr strm = filesystem::browser::get().openReadStream(url);
		if( !strm )
			return; // Error opening file

		// Allocate a string big enough to contain the source + '\0'
		unsigned int src_size = strm->size();
		boost::scoped_array<char> src_str(new char[src_size + 1]);
		strm->read(src_str.get(), src_size);
		src_str[src_size] = 0;
		source_ = src_str.get();
 
		loadSource();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int ShaderType>
	void shader_base<ShaderType>::loadSource() const
	{
		const char *src_ptr = source_.c_str();
		glShaderSource(id_, 1, &src_ptr, 0);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int ShaderType>
	void shader_base<ShaderType>::compile() const
	{
		glCompileShader(id_);
		handleCompilationErrors();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int ShaderType>
	void shader_base<ShaderType>::handleCompilationErrors() const
	{
		// First of all check if the compilation went well
		int status = GL_TRUE;
		glGetShaderiv(id_, GL_COMPILE_STATUS, &status);
		if( status == GL_TRUE )
			return; // No error

		// Retrieve log size
		int log_size = 0;
		glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &log_size);

		// Allocate a string big enough to contain the log + '\0'
		boost::scoped_array<char> log_str(new char[log_size + 1]);

		// Retrieve the log
		glGetShaderInfoLog(id_, log_size, &log_size, log_str.get());

		// TODO : throw some exception
		log::logger::log(log::EWL_CRITICAL)
			<< "====================================================================\n"
			<< "                    SHADER COMPILATION ERROR(S)                     \n"
			<< "--------------------------------------------------------------------\n"
			<< log_str.get()
			<< "===================================================================="
			<< log::logger::endl();
	}
	//----------------------------------------------------------------------------------------------

} /*ogl*/ } /*video*/ } /*djah*/
