namespace djah { namespace math {

	//----------------------------------------------------------------------------------------------
	template<typename T>
	dual_number<T>::dual_number(T _real, T _dual)
		: real_(_real)
		, dual_(_dual)
	{

	}
	//----------------------------------------------------------------------------------------------


	/**********************************************************************************************/


	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline T&		dual_number<T>::real()		 { return real_; }
	template<typename T>
	inline const T& dual_number<T>::real() const { return real_; }
	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline T&		dual_number<T>::dual()		 { return dual_; }
	template<typename T>
	inline const T& dual_number<T>::dual() const { return dual_; }
	//----------------------------------------------------------------------------------------------


	/**********************************************************************************************/


	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline dual_number<T> dual_number<T>::operator -() const
	{
		return dual_number<T>(-real_, -dual_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline dual_number<T>& dual_number<T>::operator +=(const dual_number<T> &rhs)
	{
		real_ += rhs.real();
		dual_ += rhs.dual();
		return (*this);
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline dual_number<T>& dual_number<T>::operator -=(const dual_number<T> &rhs)
	{
		real_ -= rhs.real();
		dual_ -= rhs.dual();
		return (*this);
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline dual_number<T>& dual_number<T>::operator *=(const dual_number<T> &rhs)
	{
		real_ *= rhs.real();
		dual_ = (real_ * rhs.dual()) + (dual_ * rhs.real());
		return (*this);
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline dual_number<T>& dual_number<T>::operator *=(T rhs)
	{
		real_ *= rhs;
		dual_ *= rhs;
		return (*this);
	}
	//----------------------------------------------------------------------------------------------


	/**********************************************************************************************/


	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline bool operator ==(const dual_number<T> &lhs, const dual_number<T> &rhs)
	{
		return lhs.real() == rhs.real() && lhs.dual() == rhs.dual();
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline bool operator !=(const dual_number<T> &lhs, const dual_number<T> &rhs)
	{
		return !(lhs == rhs);
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline dual_number<T> operator +(const dual_number<T> &lhs, const dual_number<T> &rhs)
	{
		return dual_number<T>(lhs) += rhs;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline dual_number<T> operator -(const dual_number<T> &lhs, const dual_number<T> &rhs)
	{
		return dual_number<T>(lhs) -= rhs;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline dual_number<T> operator *(const dual_number<T> &lhs, const dual_number<T> &rhs)
	{
		return dual_number<T>(lhs) *= rhs;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline dual_number<T> operator *(T lhs, const dual_number<T> &rhs)
	{
		return dual_number<T>(rhs) *= lhs;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline dual_number<T> operator *(const dual_number<T> &lhs, T rhs)
	{
		return dual_number<T>(lhs) *= rhs;
	}
	//----------------------------------------------------------------------------------------------

} /*math*/ } /*djah*/