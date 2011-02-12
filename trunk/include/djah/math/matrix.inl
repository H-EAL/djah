namespace djah { namespace math {
	
	//------------------------------------------------------------------------------
	template<int N, typename T>
	matrix<N,T>::matrix()
	{
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	template<int N, typename T>
	matrix<N,T>::matrix(const T (&array)[N*N])
	{
		for(int i = 0; i < N*N; ++i)
			data[i] = array[i];
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	template<int N, typename T>
	matrix<N,T>::matrix(const T (&array)[N][N])
	{
		for(int i = 0; i < N; ++i)
			for(int j = 0; j < N; ++j)
				data[i + j*N] = array[i][j];
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<int N, typename T>
	matrix<N,T>& matrix<N,T>::identity()
	{
		for(int i = 0; i < N; ++i)
			for(int j = 0; j < N; ++j)
				data[i + j*N] =  i==j ? T(1) : T(0);
		return (*this);
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	template<int N, typename T>
	matrix<N,T>& matrix<N,T>::transpose()
	{
		for(int i = 0; i < N; ++i)
			for(int j = i+1; j < N; ++j)
				std::swap(data[i + j*N], data[j + i*N]);
		return (*this);
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	template<int N, typename T>
	matrix<N,T>& matrix<N,T>::adjugate()
	{
		return (*this);
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	template<int N, typename T>
	matrix<N,T>& matrix<N,T>::invert()
	{
		const T determinant = getDeterminant();
		assert( determinant != T(0) );
		
		adjugate();
		transpose();

		*this *= T(1) / determinant;

		return (*this);
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<int N, typename T>
	matrix<N,T> matrix<N,T>::getTransposed() const
	{
		return matrix<N,T>(*this).transpose();
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	template<int N, typename T>
	matrix<N,T> matrix<N,T>::getAdjugate() const
	{
		return matrix<N,T>(*this).adjugate();
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	template<int N, typename T>
	matrix<N,T> matrix<N,T>::getInvert() const
	{
		return matrix<N,T>(*this).invert();
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	template<int N, typename T>
	T matrix<N,T>::determinant() const
	{
		if(N==2)
			return (_11 * _22) - (_12 * _21);

		T det = T(0);

		for(int i = 0; i < N; ++i)
		{
			T diag = T(1);
			T counter_diag = T(1);
			for(int j = 0; j < N; ++j)
			{
				const int kdi = (i+j) % N;
				const int kci = ((N-1) + (i-j)) % N;

				diag         *= data[kdi + j*N];
				counter_diag *= data[kci + j*N];
			}
			det += diag - counter_diag;
		}

		return det;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<int N, typename T>
	typename matrix<N,T>::row_t matrix<N,T>::row(int i) const
	{
		row_t r;
		for(int j = 0; j < N; ++j)
			r.data[j] = data[i + j*N];
		return r;
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	template<int N, typename T>
	typename matrix<N,T>::col_t matrix<N,T>::col(int j) const
	{
		col_t c;
		for(int i = 0; i < N; ++i)
			c.data[i] = data[i + j*N];
		return c;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<int N, typename T>
	inline const matrix<N,T> matrix<N,T>::operator -() const
	{
		return (*this) * T(-1);
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	template<int N, typename T>
	inline matrix<N,T>& matrix<N,T>::operator +=(const matrix<N,T> &rhs)
	{
		for(int i = 0; i < N*N; ++i)
			data[i] += rhs.data[i];
		return (*this);
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	template<int N, typename T>
	inline matrix<N,T>& matrix<N,T>::operator -=(const matrix<N,T> &rhs)
	{
		for(int i = 0; i < N*N; ++i)
			data[i] -= rhs.data[i];
		return (*this);
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	template<int N, typename T>
	inline matrix<N,T>& matrix<N,T>::operator *=(const matrix<N,T> &rhs)
	{
		matrix<N,T> rhsT(rhs.getTransposed());

		for(int i = 0; i < N; ++i)
		{
			const row_t r_i( row(i) );
			for(int j = 0; j < N; ++j)
			{
				data[i + j*N] = T(0);
				const row_t r_j( rhsT.row(j) );
				for(int k = 0; k < N; ++k)
					data[i + j*N] += r_i.data[k] * r_j.data[k];
			}
		}

		return (*this);
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	template<int N, typename T>
	inline matrix<N,T>& matrix<N,T>::operator *=(T rhs)
	{
		for(int i = 0; i < N*N; ++i)
			data[i] *= rhs;
		return (*this);
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	template<int N, typename T>
	inline matrix<N,T>& matrix<N,T>::operator /=(T rhs)
	{
		assert(rhs != T(0));
		return *this *= (T(1)/rhs);
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	/*template<int N, typename T>
	inline void print(std::ostream &out) const
	{
		for(int i = 0; i < N; ++i)
		{
			out << "[ ";
			for(int j = 0; j < N; ++j)
				out << matrix<N,T>::data_[i + j*N] << " ";
			out << "]\n";
		}
	}*/
	//------------------------------------------------------------------------------


	/******************************************************************************/


	//------------------------------------------------------------------------------
	template<int N, typename T>
	inline const matrix<N,T> operator +(const matrix<N,T> &lhs, const matrix<N,T> &rhs)
	{
		return matrix<N,T>(lhs) += rhs;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<int N, typename T>
	inline const matrix<N,T> operator -(const matrix<N,T> &lhs, const matrix<N,T> &rhs)
	{
		return matrix<N,T>(lhs) -= rhs;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<int N, typename T>
	inline const matrix<N,T> operator *(const matrix<N,T> &lhs, T rhs)
	{
		return matrix<N,T>(lhs) *= rhs;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<int N, typename T>
	inline const matrix<N,T> operator *(T lhs, const matrix<N,T> &rhs)
	{
		return rhs * lhs;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<int N, typename T>
	inline const matrix<N,T> operator /(const matrix<N,T> &lhs, T rhs)
	{
		return matrix<N,T>(lhs) /= rhs;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<int N, typename T>
	const matrix<N,T> operator *(const matrix<N,T> &lhs, const matrix<N,T> &rhs)
	{
		return matrix<N,T>(lhs) *= rhs;
	}
	//------------------------------------------------------------------------------

} /*math*/ } /*djah*/