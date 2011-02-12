namespace djah { namespace math {
	
	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	matrix<N,T>::matrix()
	{
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	matrix<N,T>::matrix(const T (&array)[N*N])
	{
		memcpy(matrix_base<N,T>::data, array, N*N*sizeof(T));
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	matrix<N,T>::matrix(const T (&array)[N][N])
	{
		memcpy(matrix_base<N,T>::data, array, N*N*sizeof(T));
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline matrix<N,T>& matrix<N,T>::identity()
	{
		memset(matrix_base<N,T>::data, 0, N*N*sizeof(T));
		for(int i = 0; i < N; ++i)
			m(i,i) = T(1);
		return (*this);
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline matrix<N,T>& matrix<N,T>::transpose()
	{
		for(int r = 0; r < N; ++r)
			for(int c = r+1; c < N; ++c)
				std::swap(m(r,c), m(c,r));
		return (*this);
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline matrix<N,T>& matrix<N,T>::adjugate()
	{
		return (*this);
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline matrix<N,T>& matrix<N,T>::invert()
	{
		const T determinant = determinant();
		assert( determinant != T(0) );
		
		adjugate();
		transpose();

		*this *= T(1) / determinant;

		return (*this);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline matrix<N,T> matrix<N,T>::getTransposed() const
	{
		return matrix<N,T>(*this).transpose();
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline matrix<N,T> matrix<N,T>::getAdjugate() const
	{
		return matrix<N,T>(*this).adjugate();
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline matrix<N,T> matrix<N,T>::getInvert() const
	{
		return matrix<N,T>(*this).invert();
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline T matrix<N,T>::determinant() const
	{
		if(N==2)
			return (m<1,1>() * m<2,2>()) - (m<1,2>() * m<2,1>());

		T det = T(0);

		for(int i = 0; i < N; ++i)
		{
			T diag = T(1);
			T counter_diag = T(1);
			for(int j = 0; j < N; ++j)
			{
				const int kdi = (i+j) % N;
				const int kci = ((N-1) + (i-j)) % N;

				diag         *= matrix_base<N,T>::data[kdi + j*N];
				counter_diag *= matrix_base<N,T>::data[kci + j*N];
			}
			det += diag - counter_diag;
		}

		return det;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline typename matrix<N,T>::row_t matrix<N,T>::row(int r) const
	{
		row_t result;
		for(int c = 0; c < N; ++c)
			result.data[c] = m(r,c);
		return result;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline typename matrix<N,T>::col_t matrix<N,T>::col(int c) const
	{
		col_t result;
		for(int r = 0; r < N; ++r)
			result.data[r] = m(r,c);
		return result;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	template<int Row, int Col>
	inline T& matrix<N,T>::m()
	{
		return matrix_base<N,T>::data[Row*N + Col];
	}
	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	template<int Row, int Col>
	inline const T& matrix<N,T>::m() const
	{
		return matrix_base<N,T>::data[Row*N + Col];
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline T& matrix<N,T>::m(int at_row, int at_col)
	{
		return matrix_base<N,T>::data[at_row*N + at_col];
	}
	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline const T& matrix<N,T>::m(int at_row, int at_col) const
	{
		return matrix_base<N,T>::data[at_row*N + at_col];
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline const matrix<N,T> matrix<N,T>::operator -() const
	{
		return (*this) * T(-1);
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline matrix<N,T>& matrix<N,T>::operator +=(const matrix<N,T> &rhs)
	{
		for(int i = 0; i < N*N; ++i)
			matrix_base<N,T>::data[i] += rhs.data[i];
		return (*this);
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline matrix<N,T>& matrix<N,T>::operator -=(const matrix<N,T> &rhs)
	{
		for(int i = 0; i < N*N; ++i)
			matrix_base<N,T>::data[i] -= rhs.data[i];
		return (*this);
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline matrix<N,T>& matrix<N,T>::operator *=(const matrix<N,T> &rhs)
	{
		matrix<N,T> rhsT(rhs.getTransposed());

		for(int r = 0; r < N; ++r)
		{
			const row_t row_lhs( row(r) );
			for(int c = 0; c < N; ++c)
			{
				m(r,c) = T(0);
				const row_t row_rhsT( rhsT.row(c) );
				for(int k = 0; k < N; ++k)
					m(r,c) += row_lhs.data[k] * row_rhsT.data[k];
			}
		}

		return (*this);
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline matrix<N,T>& matrix<N,T>::operator *=(T rhs)
	{
		for(int i = 0; i < N*N; ++i)
			matrix_base<N,T>::data[i] *= rhs;
		return (*this);
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline matrix<N,T>& matrix<N,T>::operator /=(T rhs)
	{
		assert(rhs != T(0));
		for(int i = 0; i < N*N; ++i)
			matrix_base<N,T>::data[i] /= rhs;
		return (*this);
	}
	//----------------------------------------------------------------------------------------------


	/**********************************************************************************************/


	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline const matrix<N,T> operator +(const matrix<N,T> &lhs, const matrix<N,T> &rhs)
	{
		return matrix<N,T>(lhs) += rhs;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline const matrix<N,T> operator -(const matrix<N,T> &lhs, const matrix<N,T> &rhs)
	{
		return matrix<N,T>(lhs) -= rhs;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline const matrix<N,T> operator *(const matrix<N,T> &lhs, T rhs)
	{
		return matrix<N,T>(lhs) *= rhs;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline const matrix<N,T> operator *(T lhs, const matrix<N,T> &rhs)
	{
		return rhs * lhs;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline const matrix<N,T> operator /(const matrix<N,T> &lhs, T rhs)
	{
		return matrix<N,T>(lhs) /= rhs;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	const matrix<N,T> operator *(const matrix<N,T> &lhs, const matrix<N,T> &rhs)
	{
		return matrix<N,T>(lhs) *= rhs;
	}
	//----------------------------------------------------------------------------------------------
	
	
	/**********************************************************************************************/


	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline std::ostream& operator <<(std::ostream &out, const matrix<N,T> &rhs)
	{
		for(int r = 0; r < N; ++r)
		{
			out << "[ ";
			for(int c = 0; c < N; ++c)
				out << rhs.m(r,c) << " ";
			out << "]\n";
		}

		return out;
	}
	//----------------------------------------------------------------------------------------------

} /*math*/ } /*djah*/