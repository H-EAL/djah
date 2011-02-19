namespace djah { namespace filesystem {

	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline size_t stream::read(T &data)
	{
		return read(&data, 1);
	}
	//----------------------------------------------------------------------------------------------
	template<typename T, size_t N>
	inline size_t stream::read(T (&data_array)[N])
	{
		return read(&data_array[0], N);
	}
	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline size_t stream::read(T *data_ptr, size_t count)
	{
		return readImpl(reinterpret_cast<char*>(data_ptr), count * sizeof(T));
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline size_t stream::write(const T &data)
	{
		return write(&data, 1);
	}
	//----------------------------------------------------------------------------------------------
	template<typename T, size_t N>
	inline size_t stream::write(const T (&data_array)[N])
	{
		return write(&data_array[0], N);
	}
	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline size_t stream::write(const T *data_ptr, size_t count)
	{
		return writeImpl(reinterpret_cast<const char*>(data_ptr), count * sizeof(T));
	}
	//----------------------------------------------------------------------------------------------
	template<typename Char_T>
	inline size_t stream::write(const std::basic_string<Char_T> &str)
	{
		return write(str.c_str(), str.size());
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline stream& stream::operator >>(T &toRead)
	{
		read(toRead);
		return *this;
	}
	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline stream& stream::operator <<(const T &toWrite)
	{
		write(toWrite);
		return *this;
	}
	//----------------------------------------------------------------------------------------------

} /*filesystem*/ } /*djah*/