namespace djah { namespace math {

	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	const vector<N,T> vector<N,T>::null_vector;
	//----------------------------------------------------------------------------------------------


	/**********************************************************************************************/


	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	vector<N,T>::vector(const T (&array)[N])
	{ 
		memcpy(vector_base<N,T>::data, array, N*sizeof(T));
	}
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline vector<N,T>& vector<N,T>::operator =(T (&array)[N])
	{
		memcpy(vector_base<N,T>::data, array, N*sizeof(T));
		return (*this);
	}
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline T vector<N,T>::lengthSq() const
	{
		return (*this) * (*this);
	}
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline typename vector<N,T>::float_t vector<N,T>::length() const
	{
		return sqrt( static_cast<float_t>(lengthSq()) );
	}
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline vector<N,T>& vector<N,T>::normalize()
	{
		return (*this) /= length();
	}
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline vector<N,T> vector<N,T>::getNormalized() const
	{
		return vector<N,T>(*this).normalize();
	}
	//----------------------------------------------------------------------------------------------
	
	
	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline const vector<N,T> vector<N,T>::operator -() const
	{
		vector<N,T> result;

		for(int i = 0; i < N; ++i)
			result.data[i] = -vector_base<N,T>::data[i];

		return result;
	}
	//----------------------------------------------------------------------------------------------
	
	
	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline vector<N,T>& vector<N,T>::operator +=(const vector<N,T> &rhs)
	{
		for(int i = 0; i < N; ++i)
			vector_base<N,T>::data[i] += rhs.data[i];
		return (*this);
	}
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline vector<N,T>& vector<N,T>::operator -=(const vector<N,T> &rhs)
	{
		for(int i = 0; i < N; ++i)
			vector_base<N,T>::data[i] -= rhs.data[i];
		return (*this); }
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline vector<N,T>& vector<N,T>::operator *=(T rhs)
	{
		for(int i = 0; i < N; ++i)
			vector_base<N,T>::data[i] *= rhs;
		return (*this);
	}
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline vector<N,T>& vector<N,T>::operator /=(T rhs)
	{
		assert(rhs != T(0));
		for(int i = 0; i < N; ++i)
			vector_base<N,T>::data[i] /= rhs;
		return (*this);
	}
	//----------------------------------------------------------------------------------------------


	/**********************************************************************************************/


	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline bool operator ==(const vector<N,T> &lhs, const vector<N,T> &rhs)
	{
		return std::equal(lhs.data, lhs.data+N, rhs.data);
	}
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline bool operator !=(const vector<N,T> &lhs, const vector<N,T> &rhs)
	{
		return !(lhs == rhs);
	}
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline const vector<N,T> operator +(const vector<N,T> &lhs, const vector<N,T> &rhs)
	{
		return vector<N,T>(lhs) += rhs;
	}
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline const vector<N,T> operator -(const vector<N,T> &lhs, const vector<N,T> &rhs)
	{
		return vector<N,T>(lhs) -= rhs;
	}
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline const vector<N,T> operator *(const vector<N,T> &lhs, T rhs)
	{
		return vector<N,T>(lhs) *= rhs;
	}
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	template<typename T, int N>
	inline const vector<N,T> operator *(T lhs, const vector<N,T> &rhs)
	{
		return rhs * lhs;
	}
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline const vector<N,T> operator /(const vector<N,T> &lhs, T rhs)
	{
		return vector<N,T>(lhs) /= rhs;
	}
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	template<typename T, int N>
	inline T dot(const vector<N,T> &lhs, const vector<N,T> &rhs)
	{
		T result(0);

		for(int i = 0; i < N; ++i)
			result += lhs.data[i] * rhs.data[i];

		return result;
	}
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline T operator *(const vector<N,T> &lhs, const vector<N,T> &rhs)
	{
		return dot(lhs, rhs);
	}
	//----------------------------------------------------------------------------------------------


	/**********************************************************************************************/


	//----------------------------------------------------------------------------------------------
	template<typename U, int N, typename T>
	inline const vector<N,U> cast(const vector<N,T> &op)
	{
		vector<N,U> result;
		
		for(int i = 0; i < N; ++i)
			result.data[i] = static_cast<U>(op.data[i]);

		return result;
	}
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	template<int N2, int N, typename T>
	inline const vector<N2,T> resize(const vector<N,T> &op, T padding) 
	{
		vector<N2,T> result;
		
		for(int i = 0; i < N2; ++i)
			result.data[i] = i<N ? op.data[i] : padding;
		
		return result;
	}
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	template<typename V, int N, typename T>
	inline const V vector_cast(const vector<N,T> &op, T padding) 
	{
		V result;
		
		for(int i = 0; i < V::size; ++i)
			result.data[i] = i<N ? static_cast<typename V::value_type>(op.data[i]) : padding;
		
		return result;
	}
	//----------------------------------------------------------------------------------------------

} /*math*/ } /*djah*/