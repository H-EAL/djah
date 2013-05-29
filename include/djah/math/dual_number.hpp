#ifndef DJAH_MATH_DUAL_NUMBER_HPP
#define DJAH_MATH_DUAL_NUMBER_HPP

namespace djah { namespace math {

	template<typename T>
	class dual_number
	{
	public:
		dual_number(T _real = T(0), T _dual = T(0));

		// Accessors
		T&		 real();
		const T& real() const;
		T&		 dual();
		const T& dual() const;

		// Unary minus
		dual_number<T> operator -() const;

		// Operators
		dual_number<T>& operator +=(const dual_number<T> &rhs);
		dual_number<T>& operator -=(const dual_number<T> &rhs);
		dual_number<T>& operator *=(const dual_number<T> &rhs);
		dual_number<T>& operator *=(T rhs);

	private:
		T real_;
		T dual_;
	};


	//----------------------------------------------------------------------------------------------
	// Binary operators
	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline bool operator ==(const dual_number<T> &lhs, const dual_number<T> &rhs);

	template<typename T>
	inline bool operator !=(const dual_number<T> &lhs, const dual_number<T> &rhs);

	template<typename T>
	inline dual_number<T> operator +(const dual_number<T> &lhs, const dual_number<T> &rhs);

	template<typename T>
	inline dual_number<T> operator -(const dual_number<T> &lhs, const dual_number<T> &rhs);

	template<typename T>
	inline dual_number<T> operator *(const dual_number<T> &lhs, const dual_number<T> &rhs);

	template<typename T>
	inline dual_number<T> operator *(T lhs, const dual_number<T> &rhs);

	template<typename T>
	inline dual_number<T> operator *(const dual_number<T> &lhs, T rhs);
	//----------------------------------------------------------------------------------------------

} /*math*/ } /*djah*/

#include "dual_number.inl"

#endif /* DJAH_MATH_DUAL_NUMBER_HPP */