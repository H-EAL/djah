namespace djah { namespace math {

	//--------------------------------------------------------------------------
	template<typename T>
	quaternion<T>::quaternion(const T (&array)[4])
	{
		memcpy(data, array, 4*sizeof(T));
	}
	//--------------------------------------------------------------------------
	
	
	//--------------------------------------------------------------------------
	template<typename T>
	quaternion<T>::quaternion(T _x, T _y, T _z, T _w)
	: x(_x), y(_y), z(_z), w(_w)
	{
	}
	//--------------------------------------------------------------------------
	
	
	//--------------------------------------------------------------------------
	template<typename T>
	inline T quaternion<T>::magnitude() const
	{
		return sqrt(x*x + y*y + z*z + w*w);
	}
	//--------------------------------------------------------------------------
	
	
	//--------------------------------------------------------------------------
	template<typename T>
	inline quaternion<T>& quaternion<T>::normalize()
	{
		return *this /= magnitude();
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	template<typename T>
	inline quaternion<T>& quaternion<T>::conjugate()
	{
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	template<typename T>
	inline quaternion<T> quaternion<T>::getConjugate() const
	{
		return quaternion<T>(*this).conjugate();
	}
	//--------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<typename T>
	inline const quaternion<T> quaternion<T>::operator -() const
	{
		return (*this) * T(-1);
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<typename T>
	inline quaternion<T>& quaternion<T>::operator +=(const quaternion<T> &rhs)
	{
		for(size_t i = 0; i < 4; ++i)
			data[i] += rhs.data[i];

		return *this;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<typename T>
	inline quaternion<T>& quaternion<T>::operator -=(const quaternion<T> &rhs)
	{
		for(size_t i = 0; i < 4; ++i)
			data[i] -= rhs.data[i];

		return *this;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<typename T>
	inline quaternion<T>& quaternion<T>::operator *=(T rhs)
	{
		for(size_t i = 0; i < 4; ++i)
			data[i] *= rhs;
		return *this;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<typename T>
	inline quaternion<T>& quaternion<T>::operator /=(T rhs)
	{
		assert(rhs != T(0));
		return *this *= (T(1)/rhs);
	}
	//------------------------------------------------------------------------------




	/******************************************************************************/




	//------------------------------------------------------------------------------
	template<typename T>
	inline const quaternion<T> operator +(const quaternion<T> &lhs, const quaternion<T> &rhs)
	{
		return quaternion<T>(lhs) += rhs;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<typename T>
	inline const quaternion<T> operator -(const quaternion<T> &lhs, const quaternion<T> &rhs)
	{
		return quaternion<T>(lhs) -= rhs;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<typename T>
	inline const quaternion<T> operator *(const quaternion<T> &lhs, T rhs)
	{
		return quaternion<T>(lhs) *= rhs;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<typename T>
	inline const quaternion<T> operator *(T lhs, const quaternion<T> &rhs)
	{
		return rhs * lhs;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<typename T>
	inline const quaternion<T> operator /(const quaternion<T> &lhs, T rhs)
	{
		return quaternion<T>(lhs) /= rhs;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<typename T>
	inline const quaternion<T> operator *(const quaternion<T> &lhs, const quaternion<T> &rhs)
	{
		const vector<3,T> v1(lhs.x, lhs.y, lhs.z);
		const vector<3,T> v2(rhs.x, rhs.y, rhs.z);
		const vector<3,T> vR(v2*lhs.w + v1*rhs.w + cross(v1,v2));

		return quaternion<T>(vR.x, vR.y, vR.z, lhs.w*rhs.w - v1*v2);
	}
	//------------------------------------------------------------------------------

} /*math*/ } /*djah*/