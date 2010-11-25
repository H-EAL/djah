namespace djah { namespace math { namespace detail {

	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	vector_base<SIZE,T>::vector_base()
	{
		std::fill(begin(), end(), T());
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	vector_base<SIZE,T>::vector_base(const T (&array)[SIZE])
	{
		std::copy(begin(), end(), data_);
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline const T* vector_base<SIZE,T>::data() const
	{
		return data_;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline const T& vector_base<SIZE,T>::operator [](size_t i) const
	{
		assert(i < SIZE);
		return data_[i];
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline T& vector_base<SIZE,T>::operator [](size_t i) 
	{
		assert(i < SIZE);
		return data_[i];
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline vector_base<SIZE,T>::operator const T *() const 
	{
		return data_;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline T* vector_base<SIZE,T>::at(size_t i) 
	{
		assert(i < SIZE);
		return &data_[i];
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline const T* vector_base<SIZE,T>::at(size_t i) const
	{
		assert(i < SIZE);
		return &data_[i];
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline typename vector_base<SIZE,T>::iterator vector_base<SIZE,T>::begin()
	{ return data_; }
	template<size_t SIZE, typename T>
	inline typename vector_base<SIZE,T>::iterator vector_base<SIZE,T>::end()  
	{ return data_ + SIZE; }
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline typename vector_base<SIZE,T>::iterator_to_const vector_base<SIZE,T>::begin() const
	{ return data_; }
	template<size_t SIZE, typename T>
	inline typename vector_base<SIZE,T>::iterator_to_const vector_base<SIZE,T>::end()   const
	{ return data_ + SIZE; }
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline void vector_base<SIZE,T>::print(std::ostream &out) const
	{
		out << "( ";
		std::copy(begin(), end(), std::ostream_iterator<T>(out, " "));
		out << ")";
	}
	//------------------------------------------------------------------------------




	/******************************************************************************/



	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline std::ostream& operator <<(std::ostream &out, const vector_base<SIZE,T> &v)
	{
		v.print(out);
		return out;
	}
	//------------------------------------------------------------------------------

} /*detail*/ } /*math*/ } /*djah*/