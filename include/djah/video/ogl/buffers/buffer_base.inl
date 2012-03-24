namespace djah { namespace video { namespace ogl {

	//----------------------------------------------------------------------------------------------
	template<typename T>
	size_t buffer_base::read(T *data_ptr, size_t count, int offset)
	{
		bind();

		const int bytes = count * sizeof(T);
		glGetBufferSubData(target_, offset * sizeof(T), bytes, data_ptr);

		unbind();

		return count;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T, size_t N>
	size_t buffer_base::read(T (&data_array)[N], int offset)
	{
		return read(&data_array[0], N, offset);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	size_t buffer_base::write(const T *data_ptr, size_t count, int offset)
	{
		bind();

		const int bytes = count * sizeof(T);
		glBufferSubData(target_, offset * sizeof(T), bytes, data_ptr);

		unbind();

		bytes_per_elem_ = sizeof(T);
		data_type_ = system::opengl::type<T>::Type;

		return count;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T, size_t N>
	size_t buffer_base::write(const T (&data_array)[N], int offset)
	{
		return write(&data_array[0], N, offset);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	buffer_base& buffer_base::operator >>(T &toRead)
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
	template<typename T>
	buffer_base& buffer_base::operator <<(const T &toWrite)
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

} /*ogl*/ } /*video*/ } /*djah*/
