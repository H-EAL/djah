namespace djah { namespace math {
	
	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline matrix<M,N,T>::matrix()
	{
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline matrix<M,N,T>::matrix(const T (&array)[M*N])
	{
		memcpy(matrix_base<M,N,T>::data, array, M*N*sizeof(T));
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline matrix<M,N,T>::matrix(const T (&array)[M][N])
	{
		memcpy(matrix_base<M,N,T>::data, array, M*N*sizeof(T));
	}
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline matrix<M,N,T>& matrix<M,N,T>::operator =(T (&array)[M*N])
	{
		memcpy(matrix_base<M,N,T>::data, array, M*N*sizeof(T));
		return (*this);
	}
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline matrix<M,N,T>& matrix<M,N,T>::operator =(T (&array)[M][N])
	{
		memcpy(matrix_base<M,N,T>::data, array, M*N*sizeof(T));
		return (*this);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline matrix<M,N,T>& matrix<M,N,T>::toIdentity()
	{
		assert(M==N);
		memset(matrix_base<M,N,T>::data, 0, M*N*sizeof(T));
		for(int i = 0; i < M; ++i)
			m(i,i) = T(1);
		return (*this);
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline matrix<M,N,T>& matrix<M,N,T>::transpose()
	{
		assert(M==N);
		for(int r = 0; r < M; ++r)
			for(int c = r+1; c < N; ++c)
				std::swap(m(r,c), m(c,r));
		return (*this);
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline matrix<M,N,T>& matrix<M,N,T>::adjugate()
	{
		assert(M==N);
		return (*this);
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline matrix<M,N,T>& matrix<M,N,T>::invert()
	{
		const T det = determinant();
		assert( det != T(0) );
		
		adjugate();
		transpose();

		*this *= T(1) / det;

		return (*this);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline matrix<M,N,T> matrix<M,N,T>::getTransposed() const
	{
		return matrix<M,N,T>(*this).transpose();
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline matrix<M,N,T> matrix<M,N,T>::getAdjugate() const
	{
		return matrix<M,N,T>(*this).adjugate();
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline matrix<M,N,T> matrix<M,N,T>::getInvert() const
	{
		return matrix<M,N,T>(*this).invert();
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline T matrix<M,N,T>::determinant() const
	{
		assert(M==N);

		if(N==2)
			return (m<1,1>() * m<2,2>()) - (m<1,2>() * m<2,1>());

		T det = T(0);

		for(int r = 0; r < M; ++r)
		{
			T diag = T(1);
			T counter_diag = T(1);
			for(int c = 0; c < N; ++c)
			{
				const int kdr = (r+c) % M;
				const int kcr = ((M-1) + (r-c)) % M;

				diag         *= matrix_base<M,N,T>::data[kdr + c*N];
				counter_diag *= matrix_base<M,N,T>::data[kcr + c*N];
			}
			det += diag - counter_diag;
		}

		return det;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline typename matrix<M,N,T>::row_t matrix<M,N,T>::row(int r) const
	{
		row_t result;
		for(int c = 0; c < N; ++c)
			result.data[c] = m(r,c);
		return result;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline typename matrix<M,N,T>::col_t matrix<M,N,T>::col(int c) const
	{
		col_t result;
		for(int r = 0; r < M; ++r)
			result.data[r] = m(r,c);
		return result;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	template<int Row, int Col>
	inline T& matrix<M,N,T>::m()
	{
		return matrix_base<M,N,T>::data[Row][Col];
	}
	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	template<int Row, int Col>
	inline const T& matrix<M,N,T>::m() const
	{
		return matrix_base<M,N,T>::data[Row][Col];
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline T& matrix<M,N,T>::m(int at_row, int at_col)
	{
		return matrix_base<M,N,T>::data[at_row][at_col];
	}
	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline const T& matrix<M,N,T>::m(int at_row, int at_col) const
	{
		return matrix_base<M,N,T>::data[at_row][at_col];
	}
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline T* matrix<M,N,T>::operator [](unsigned int r)
	{
		return &matrix_base<M,N,T>::data[r][0];
	}
	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline const T* matrix<M,N,T>::operator [](unsigned int r) const
	{
		return &matrix_base<M,N,T>::data[r][0];
	}
	//----------------------------------------------------------------------------------------------
	

	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline const matrix<M,N,T> matrix<M,N,T>::operator -() const
	{
		return (*this) * T(-1);
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline matrix<M,N,T>& matrix<M,N,T>::operator +=(const matrix<M,N,T> &rhs)
	{
		for(int i = 0; i < M*N; ++i)
			matrix_base<M,N,T>::data[i] += rhs.data[i];
		return (*this);
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline matrix<M,N,T>& matrix<M,N,T>::operator -=(const matrix<M,N,T> &rhs)
	{
		for(int r = 0; r < M; ++r)
			for(int c = 0; c < N; ++c)
				matrix_base<M,N,T>::data[r][c] -= rhs.data[r][c];
		return (*this);
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline matrix<M,N,T>& matrix<M,N,T>::operator *=(const matrix<M,N,T> &rhs)
	{
		assert(M==N);
		matrix<M,N,T> rhsT(rhs.getTransposed());

		for(int r = 0; r < M; ++r)
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
	template<int M, int N, typename T>
	inline matrix<M,N,T>& matrix<M,N,T>::operator *=(T rhs)
	{
		for(int r = 0; r < M; ++r)
			for(int c = 0; c < N; ++c)
				matrix_base<M,N,T>::data[r][c] *= rhs;
		return (*this);
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline matrix<M,N,T>& matrix<M,N,T>::operator /=(T rhs)
	{
		assert(rhs != T(0));
		for(int r = 0; r < M; ++r)
			for(int c = 0; c < N; ++c)
				matrix_base<M,N,T>::data[r][c] /= rhs;
		return (*this);
	}
	//----------------------------------------------------------------------------------------------


	/**********************************************************************************************/


	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline const matrix<M,N,T> operator +(const matrix<M,N,T> &lhs, const matrix<M,N,T> &rhs)
	{
		return matrix<M,N,T>(lhs) += rhs;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline const matrix<M,N,T> operator -(const matrix<M,N,T> &lhs, const matrix<M,N,T> &rhs)
	{
		return matrix<M,N,T>(lhs) -= rhs;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline const matrix<M,N,T> operator *(const matrix<M,N,T> &lhs, T rhs)
	{
		return matrix<M,N,T>(lhs) *= rhs;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline const matrix<M,N,T> operator *(T lhs, const matrix<M,N,T> &rhs)
	{
		return rhs * lhs;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline const matrix<M,N,T> operator /(const matrix<M,N,T> &lhs, T rhs)
	{
		return matrix<M,N,T>(lhs) /= rhs;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	const matrix<N,N,T> operator *(const matrix<N,N,T> &lhs, const matrix<N,N,T> &rhs)
	{
		return matrix<N,N,T>(lhs) *= rhs;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<int M, int N, int P, typename T>
	const matrix<M,P,T> operator *(const matrix<M,N,T> &lhs, const matrix<N,P,T> &rhs)
	{
		matrix<M,P,T> result;

		for(int r = 0; r < M; ++r)
		{
			const matrix<M,N,T>::row_t row_lhs( lhs.row(r) );
			for(int c = 0; c < P; ++c)
			{
				result.m(r,c) = T(0);
				const matrix<N,P,T>::col_t col_rhs( rhs.col(c) );
				for(int k = 0; k < N; ++k)
					result.m(r,c) += row_lhs.data[k] * col_rhs.data[k];
			}
		}

		return result;
	}
	//----------------------------------------------------------------------------------------------
	
	
	/**********************************************************************************************/


	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	inline std::ostream& operator <<(std::ostream &out, const matrix<M,N,T> &rhs)
	{
		for(int r = 0; r < M; ++r)
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