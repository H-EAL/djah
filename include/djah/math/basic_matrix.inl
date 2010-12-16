namespace djah { namespace math {

	using namespace detail;
	
	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	basic_matrix<SIZE,T>::basic_matrix()
	{
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	basic_matrix<SIZE,T>::basic_matrix(const T (&array)[SIZE*SIZE])
	{
		for(size_t i = 0; i < SIZE*SIZE; ++i)
				vector_base::data_[i] = array[i];
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	basic_matrix<SIZE,T>::basic_matrix(const T (&array)[SIZE][SIZE])
	{
		for(size_t i = 0; i < SIZE; ++i)
			for(size_t j = 0; j < SIZE; ++j)
				data_[i + j*SIZE] = array[i][j];
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	template<size_t I, size_t J>
	T& basic_matrix<SIZE,T>::m() { return vector_base::data_[I-1 + (J-1)*SIZE]; }
	template<size_t SIZE, typename T>
	template<size_t I, size_t J>
	const T& basic_matrix<SIZE,T>::m() const { return vector_base::data_[I-1 * (J-1)*SIZE]; }
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	void basic_matrix<SIZE,T>::identity()
	{
		for(size_t i = 0; i < SIZE; ++i)
			for(size_t j = 0; j < SIZE; ++j)
				vector_base::data_[i + j * SIZE] = i==j ? T(1) : T(0);
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	void basic_matrix<SIZE,T>::transpose()
	{
		for(size_t i = 0; i < SIZE; ++i)
			for(size_t j = i+1; j < SIZE; ++j)
				std::swap(vector_base::data_[i + j*SIZE], vector_base::data_[j + i*SIZE]);
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	void basic_matrix<SIZE,T>::adjugate()
	{
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	void basic_matrix<SIZE,T>::invert()
	{
		const T determinant = getDeterminant();
		assert( determinant != T(0) );
		
		adjugate();
		transpose();

		*this *= T(1) / determinant;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	T basic_matrix<SIZE,T>::getDeterminant() const
	{
		if( SIZE == 2 )
			return m<1,1>() * m<2,2>() - m<1,2>() * m<2,1>();

		T det = T(0);

		for(size_t i = 0; i < SIZE; ++i)
		{
			T diag = T(1);
			T counter_diag = T(1);
			for(size_t j = 0; j < SIZE; ++j)
			{
				const size_t kdi = (i+j) % SIZE;
				const size_t kci = ((SIZE-1) + (i-j)) % SIZE;

				diag         *= vector_base::data_[kdi + j*SIZE];
				counter_diag *= vector_base::data_[kci + j*SIZE];
			}
			det += diag - counter_diag;
		}

		return det;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	typename basic_matrix<SIZE,T>::row_t basic_matrix<SIZE,T>::row(size_t i) const
	{
		row_t r;
		for(size_t j = 0; j < SIZE; ++j)
			r[j] = vector_base::data_[i + j*SIZE];
		return r;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	typename basic_matrix<SIZE,T>::col_t basic_matrix<SIZE,T>::col(size_t j) const
	{
		col_t c;
		for(size_t i = 0; i < SIZE; ++i)
			c[i] = vector_base::data_[i + j*SIZE];
		return c;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline const basic_matrix<SIZE,T> basic_matrix<SIZE,T>::operator -() const
	{
		return (*this) * T(-1);
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline basic_matrix<SIZE,T>& basic_matrix<SIZE,T>::operator +=(const basic_matrix<SIZE,T> &rhs)
	{
		for(size_t i = 0; i < SIZE; ++i)
			for(size_t j = 0; j < SIZE; ++j)
				vector_base::data_[i + j*SIZE] += rhs.data_[i + j*SIZE];

		return *this;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline basic_matrix<SIZE,T>& basic_matrix<SIZE,T>::operator -=(const basic_matrix<SIZE,T> &rhs)
	{
		for(size_t i = 0; i < SIZE; ++i)
			for(size_t j = 0; j < SIZE; ++j)
				vector_base::data_[i + j*SIZE] -= rhs.data_[i + j*SIZE];

		return *this;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline basic_matrix<SIZE,T>& basic_matrix<SIZE,T>::operator *=(const basic_matrix<SIZE,T> &rhs)
	{
		typedef basic_matrix<SIZE,T> matrix_t;

		matrix_t rhsT(rhs);
		rhsT.transpose();

		for(size_t i = 0; i < SIZE; ++i)
		{
			const matrix_t::row_t r_i( row(i) );
			for(size_t j = 0; j < SIZE; ++j)
			{
				vector_base::data_[i + j*SIZE] = T(0);
				const matrix_t::row_t r_j( rhsT.row(j) );
				for(size_t k = 0; k < SIZE; ++k)
					vector_base::data_[i + j*SIZE] += r_i[k] * r_j[k];
			}
		}
		return *this;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline basic_matrix<SIZE,T>& basic_matrix<SIZE,T>::operator *=(T rhs)
	{
		std::transform(vector_base<SIZE,T>::begin(), vector_base<SIZE,T>::end(), vector_base::data_, std::bind2nd(std::multiplies<T>(), rhs));
		return *this;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline basic_matrix<SIZE,T>& basic_matrix<SIZE,T>::operator /=(T rhs)
	{
		assert(rhs != T(0));
		return *this *= (T(1)/rhs);
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline T* basic_matrix<SIZE,T>::at(size_t i, size_t j) 
	{
		assert(i < SIZE && j < SIZE);
		return &vector_base::data_[i + j*SIZE];
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline const T* basic_matrix<SIZE,T>::at(size_t i, size_t j) const
	{
		assert(i < SIZE && j < SIZE);
		return &vector_base::data_[i + j*SIZE];
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline void basic_matrix<SIZE,T>::print(std::ostream &out) const
	{
		for(size_t i = 0; i < SIZE; ++i)
		{
			out << "[ ";
			for(size_t j = 0; j < SIZE; ++j)
				out << basic_matrix<SIZE,T>::data_[i + j*SIZE] << " ";
			out << "]\n";
		}
	}
	//------------------------------------------------------------------------------




	/******************************************************************************/




	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline const basic_matrix<SIZE,T> operator +(const basic_matrix<SIZE,T> &lhs, const basic_matrix<SIZE,T> &rhs)
	{
		return basic_matrix<SIZE,T>(lhs) += rhs;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline const basic_matrix<SIZE,T> operator -(const basic_matrix<SIZE,T> &lhs, const basic_matrix<SIZE,T> &rhs)
	{
		return basic_matrix<SIZE,T>(lhs) -= rhs;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline const basic_matrix<SIZE,T> operator *(const basic_matrix<SIZE,T> &lhs, T rhs)
	{
		return basic_matrix<SIZE,T>(lhs) *= rhs;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline const basic_matrix<SIZE,T> operator *(T lhs, const basic_matrix<SIZE,T> &rhs)
	{
		return rhs * lhs;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline const basic_matrix<SIZE,T> operator /(const basic_matrix<SIZE,T> &lhs, T rhs)
	{
		return basic_matrix<SIZE,T>(lhs) /= rhs;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	const basic_matrix<SIZE,T> operator *(const basic_matrix<SIZE,T> &lhs, const basic_matrix<SIZE,T> &rhs)
	{
		return basic_matrix<SIZE,T>(lhs) *= rhs;
	}
	//------------------------------------------------------------------------------

} /*math*/ } /*djah*/