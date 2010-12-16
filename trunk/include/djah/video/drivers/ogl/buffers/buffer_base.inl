namespace djah { namespace video { namespace drivers { namespace ogl {

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