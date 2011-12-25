#ifndef DJAH_MATH_VECTOR_HPP
#define DJAH_MATH_VECTOR_HPP

#include <cmath>
#include <cassert>
#include <algorithm>

namespace djah { namespace math {

	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	struct vector_base
	{
		vector_base() { memset(data, 0, N*sizeof(T)); }
		T data[N];
	};
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	namespace {

		// Length type of vector<double> is double any other vector<T> is float
		template<typename F>	struct length_type         { typedef float  float_t; };
		template<>				struct length_type<double> { typedef double float_t; };

	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	class vector
		: public vector_base<N,T>
	{
	public:
	
		static const int size = N;
		typedef T value_type;

		// Default constructor -> null vector
		vector() {}

		// Constructors from values, only the needed one is actually created
		vector(T _x, T _y) : vector_base<N,T>(_x, _y) {}
		vector(T _x, T _y, T _z) : vector_base<N,T>(_x, _y, _z) {}
		vector(T _x, T _y, T _z, T _w) : vector_base<N,T>(_x, _y, _z, _w) {}

		// Constructor from array
		vector(const T (&array)[N]);

		// Assignation from an array
		vector<N,T>& operator =(T (&array)[N]);

		typedef typename length_type<T>::float_t float_t;

		// Vector math
		T				lengthSq()		const;
		float_t			length()		const;
		vector<N,T>&	normalize();
		vector<N,T>		getNormalized()	const;

		// Dot product
		T dot(const vector<N,T> &v) const;

		// Unary minus
		const vector<N,T> operator -() const;

		// Operators
		vector<N,T>& operator +=(const vector<N,T> &rhs);
		vector<N,T>& operator -=(const vector<N,T> &rhs);
		vector<N,T>& operator *=(T rhs);
		vector<N,T>& operator /=(T rhs);

		// Accessors
		T&		 operator [](unsigned int i);
		const T& operator [](unsigned int i) const;

		// Usefull constants
		static const vector<N,T> null_vector;
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	// Binary operators
	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline bool operator ==(const vector<N,T> &lhs, const vector<N,T> &rhs);

	template<int N, typename T>
	inline bool operator !=(const vector<N,T> &lhs, const vector<N,T> &rhs);

	template<int N, typename T>
	inline const vector<N,T> operator +(const vector<N,T> &lhs, const vector<N,T> &rhs);

	template<int N, typename T>
	inline const vector<N,T> operator -(const vector<N,T> &lhs, const vector<N,T> &rhs);

	template<int N, typename T>
	inline const vector<N,T> operator *(const vector<N,T> &lhs, T rhs);
	
	template<int N, typename T>
	inline const vector<N,T> operator *(T lhs, const vector<N,T> &rhs);

	template<int N, typename T>
	inline const vector<N,T> operator /(const vector<N,T> &lhs, T rhs);

	template<int N, typename T>
	inline T operator *(const vector<N,T> &lhs, const vector<N,T> &rhs);
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	// Cast functions
	//----------------------------------------------------------------------------------------------
	template<typename U, int N, typename T>
	inline const vector<N,U> cast(const vector<N,T> &op);

	template<int N2, int N, typename T>
	inline const vector<N2,T> resize(const vector<N,T> &op, T padding = 0);

	template<typename V, int N, typename T>
	inline const V vector_cast(const vector<N,T> &op, T padding = 0);
	//----------------------------------------------------------------------------------------------

} /*math*/ } /*djah*/

#include "vector.inl"

#endif /* DJAH_MATH_VECTOR_HPP */