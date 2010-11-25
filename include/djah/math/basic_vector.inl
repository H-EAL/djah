namespace djah { namespace math {

	using namespace detail;
	
	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	basic_vector<SIZE,T>::basic_vector()
	{
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	basic_vector<SIZE,T>::basic_vector(const T (&array)[SIZE])
		: vector_base<SIZE,T>(array)
	{
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	basic_vector<SIZE,T>::basic_vector(const detail::vector_base<SIZE,T> &v)
		: vector_base<SIZE,T>(v)
	{
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline T basic_vector<SIZE,T>::lengthSq() const
	{
		return (*this) * (*this);
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline float basic_vector<SIZE,T>::length() const
	{
		return ::sqrt(float(lengthSq()));
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline basic_vector<SIZE,T>& basic_vector<SIZE,T>::normalize()
	{
		return *this /= T(length());
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline basic_vector<SIZE,T> basic_vector<SIZE,T>::getNormalized() const
	{
		return basic_vector<SIZE,T>(*this).normalize();
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline const basic_vector<SIZE,T> basic_vector<SIZE,T>::operator -() const
	{
		return (*this) * T(-1);
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline basic_vector<SIZE,T>& basic_vector<SIZE,T>::operator +=(const basic_vector<SIZE,T> &rhs)
	{
		for(size_t i = 0; i < SIZE; ++i)
			vector_base::data_[i] += rhs.data_[i];

		return *this;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline basic_vector<SIZE,T>& basic_vector<SIZE,T>::operator -=(const basic_vector<SIZE,T> &rhs)
	{
		for(size_t i = 0; i < SIZE; ++i)
			vector_base::data_[i] -= rhs.data_[i];

		return *this;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline basic_vector<SIZE,T>& basic_vector<SIZE,T>::operator *=(T rhs)
	{
		std::transform
		(
			vector_base::begin(),
			vector_base::end(),
			stdext::checked_array_iterator<T*>(vector_base::data_, SIZE),
			std::bind2nd(std::multiplies<T>(), rhs)
		);
		return *this;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline basic_vector<SIZE,T>& basic_vector<SIZE,T>::operator /=(T rhs)
	{
		assert(rhs != T(0));
		return *this *= (T(1)/rhs);
	}
	//------------------------------------------------------------------------------




	/******************************************************************************/




	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline const basic_vector<SIZE,T> operator +(const basic_vector<SIZE,T> &lhs, const basic_vector<SIZE,T> &rhs)
	{
		return basic_vector<SIZE,T>(lhs) += rhs;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline const basic_vector<SIZE,T> operator -(const basic_vector<SIZE,T> &lhs, const basic_vector<SIZE,T> &rhs)
	{
		return basic_vector<SIZE,T>(lhs) -= rhs;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline const basic_vector<SIZE,T> operator *(const basic_vector<SIZE,T> &lhs, T rhs)
	{
		return basic_vector<SIZE,T>(lhs) *= rhs;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline const basic_vector<SIZE,T> operator *(T lhs, const basic_vector<SIZE,T> &rhs)
	{
		return rhs * lhs;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline const basic_vector<SIZE,T> operator /(const basic_vector<SIZE,T> &lhs, T rhs)
	{
		return basic_vector<SIZE,T>(lhs) /= rhs;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline const T operator *(const basic_vector<SIZE,T> &lhs, const basic_vector<SIZE,T> &rhs)
	{
		T result(0);
		for(size_t i = 0; i < SIZE; ++i)
			result += lhs[i] * rhs[i];
		return result;
	}
	//------------------------------------------------------------------------------

} /*math*/ } /*djah*/
