namespace djah { namespace fs {

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
		return readImpl(reinterpret_cast<byte*>(data_ptr), count * sizeof(T));
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
		return writeImpl(reinterpret_cast<const byte*>(data_ptr), count * sizeof(T));
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

} /*fs*/ } /*djah*/