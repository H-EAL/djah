namespace djah { namespace video { namespace drivers { namespace ogl {

	//----------------------------------------------------------------------------------------------
	template<int Target>
	buffer_base<Target>::buffer_base(size_t size, E_BUFFER_USAGE usage, bool auto_alloc)
		: usage_(usage)
		, size_(size)
		, bytes_per_elem_(0)
		, data_type_(0)
		, allocated_(false)
	{
		aquire();
		if( auto_alloc )
			allocMemory();
	}
	//----------------------------------------------------------------------------------------------
	

	//----------------------------------------------------------------------------------------------
	template<int Target>
	buffer_base<Target>::~buffer_base()
	{
		release();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int Target>
	void buffer_base<Target>::aquire()
	{
		DJAH_TEST_FOR_OPENGL_ERRORS
		(
			glGenBuffers(1, &id_)
		);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int Target>
	void buffer_base<Target>::release()
	{
		glDeleteBuffers(1, &id_);
		id_ = INVALID_ID;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int Target>
	bool buffer_base<Target>::isValidResource() const
	{
		return glIsBuffer(id_) != 0;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int Target>
	void buffer_base<Target>::bind() const
	{
		assert(id_ != INVALID_ID);
		glBindBuffer(Target, id_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int Target>
	void buffer_base<Target>::unbind()
	{
		glBindBuffer(Target, 0);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int Target>
	size_t buffer_base<Target>::size() const
	{
		return size_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int Target>
	size_t buffer_base<Target>::count() const
	{
		return size_ / bytes_per_elem_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int Target>
	u32 buffer_base<Target>::dataType() const
	{
		return data_type_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int Target>
	void buffer_base<Target>::allocMemory()
	{
		if( allocated_ )
			return;

		bind();
		glBufferData(Target, size_, 0, usage_);
		unbind();

		allocated_ = true;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int Target>
	template<typename T>
	size_t buffer_base<Target>::read(T *data_ptr, size_t count, int offset)
	{
		bind();

		const int bytes = count * sizeof(T);
		glGetBufferSubData(Target, offset * sizeof(T), bytes, data_ptr);

		unbind();

		return count;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int Target>
	template<typename T, size_t N>
	size_t buffer_base<Target>::read(T (&data_array)[N], int offset)
	{
		return read(&data_array[0], N, offset);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int Target>
	template<typename T>
	size_t buffer_base<Target>::write(const T *data_ptr, size_t count, int offset)
	{
		bind();

		const int bytes = count * sizeof(T);
		glBufferSubData(Target, offset * sizeof(T), bytes, data_ptr);

		unbind();

		bytes_per_elem_ = sizeof(T);
		data_type_ = gl_type<T>::Type;

		return count;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int Target>
	template<typename T, size_t N>
	size_t buffer_base<Target>::write(const T (&data_array)[N], int offset)
	{
		return write(&data_array[0], N, offset);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int Target>
	void buffer_base<Target>::lock()
	{
		bind();
		cursor_ = reinterpret_cast<byte*>(glMapBuffer(Target, GL_WRITE_ONLY));
		cursor_count_ = 0;
		locked_ = true;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int Target>
	template<typename T>
	buffer_base<Target>& buffer_base<Target>::operator >>(T &toRead)
	{
		assert(locked_ && cursor_);
		assert((cursor_count_ + sizeof(T)) <= size_);

		toRead = *reinterpret_cast<T*>(cursor_);
		cursor_ += sizeof(T);
		cursor_count_ += sizeof(T);

		return *this;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int Target>
	template<typename T>
	buffer_base<Target>& buffer_base<Target>::operator <<(const T &toWrite)
	{
		assert(locked_ && cursor_);
		assert((cursor_count_ + sizeof(T)) <= size_);

		*reinterpret_cast<T*>(cursor_) = toWrite;
		cursor_ += sizeof(T);
		cursor_count_ += sizeof(T);

		bytes_per_elem_ = sizeof(T);
		data_type_ = gl_type<T>::Type;

		return *this;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int Target>
	void buffer_base<Target>::unlock()
	{
		glUnmapBuffer(Target);
		unbind();
		cursor_ = 0;
		cursor_count_ = 0;
		locked_ = false;
	}
	//----------------------------------------------------------------------------------------------

} /*ogl*/ } /*drivers*/ } /*video*/ } /*djah*/