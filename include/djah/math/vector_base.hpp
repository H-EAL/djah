#include <iostream>
#include <cmath>
#include <algorithm>
#include <functional>
using namespace std;

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
template<int N, typename T>
class vector
	: public vector_base<N,T>
{
public:
	
	static const int size = N;
	typedef T value_type;

	vector() {}
	vector(T _x, T _y) : vector_base<N,T>(_x, _y) {}
	vector(T _x, T _y, T _z) : vector_base<N,T>(_x, _y, _z) {}
	vector(T _w, T _x, T _y, T _z) : vector_base<N,T>(_w, _x, _y, _z) {}

	vector(T (&array)[N]) { memcpy(vector_base<N,T>::data, array, N*sizeof(T)); }

	vector<N,T>& operator =(T (&array)[N]) { memcpy(vector_base<N,T>::data, array, N*sizeof(T)); return (*this); }

	//template<typename F>	struct type			{ typedef float		float_t; };
	//template<>				struct type<double> { typedef double	float_t; };
	//typedef typename type<T>::float_t float_t;
	typedef float float_t;

	T				lengthSq()		const	{ return (*this) * (*this); }
	float_t			length()		const	{ return sqrt(static_cast<float_t>(lengthSq())); }
	vector<N,T>&	normalize()				{ return (*this) /= length(); }
	vector<N,T>		getNormalized()	const	{ return vector<N,T>(*this).normalize(); }

	const vector<N,T> operator -() const { vector<N,T> result; for(int i = 0; i < N; ++i) result.data[i] = -vector_base<N,T>::data[i]; return result; }

	vector<N,T>& operator +=(const vector<N,T> &rhs) { for(int i = 0; i < N; ++i) vector_base<N,T>::data[i] += rhs.data[i]; return (*this); }
	vector<N,T>& operator -=(const vector<N,T> &rhs) { for(int i = 0; i < N; ++i) vector_base<N,T>::data[i] -= rhs.data[i]; return (*this); }
	vector<N,T>& operator *=(T rhs) { for(int i = 0; i < N; ++i) vector_base<N,T>::data[i] *= rhs; return (*this); }
	vector<N,T>& operator /=(T rhs) { for(int i = 0; i < N; ++i) vector_base<N,T>::data[i] /= rhs; return (*this); }

	static const vector<N,T> null_vector;
};
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
template<int N, typename T>
inline bool operator ==(const vector<N,T> &lhs, const vector<N,T> &rhs)
{
	return std::equal(lhs.data, lhs.data+N, rhs.data);
}

template<int N, typename T>
inline bool operator !=(const vector<N,T> &lhs, const vector<N,T> &rhs)
{
	return !(lhs == rhs);
}

template<int N, typename T>
inline const vector<N,T> operator +(const vector<N,T> &lhs, const vector<N,T> &rhs)
{
	return vector<N,T>(lhs) += rhs;
}

template<int N, typename T>
inline const vector<N,T> operator -(const vector<N,T> &lhs, const vector<N,T> &rhs)
{
	return vector<N,T>(lhs) -= rhs;
}

template<int N, typename T>
inline const vector<N,T> operator *(const vector<N,T> &lhs, T rhs)
{
	return vector<N,T>(lhs) *= rhs;
}

template<typename T, int N>
inline const vector<N,T> operator *(T lhs, const vector<N,T> &rhs)
{
	return rhs * lhs;
}

template<int N, typename T>
inline const vector<N,T> operator /(const vector<N,T> &lhs, T rhs)
{
	return vector<N,T>(lhs) /= rhs;
}

template<typename T, int N>
inline const vector<N,T> dot(const vector<N,T> &lhs, const vector<N,T> &rhs)
{
	T result(0);
	for(int i = 0; i < N; ++i)
		result += lhs.data[i] * rhs.data[i];
	return result;
}

template<int N, typename T>
inline const vector<N,T> operator *(const vector<N,T> &lhs, const vector<N,T> &rhs)
{
	return dot(lhs, rhs);
}
//----------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------
template<typename U, int N, typename T>
inline const vector<N,U> cast(const vector<N,T> &op)
{ vector<N,U> result; for(int i = 0; i < N; ++i) result.data[i] = static_cast<U>(op.data[i]); return result; }

template<int N2, int N, typename T>
inline const vector<N2,T> resize(const vector<N,T> &op, T padding = 0) 
{ vector<N2,T> result; for(int i = 0; i < N2; ++i) result.data[i] = i<N ? op.data[i]:padding; return result; }

template<typename V, int N, typename T>
inline const V vector_cast(const vector<N,T> &op, T padding = 0) 
{ V result; for(int i = 0; i < V::size; ++i) result.data[i] = i<N ? static_cast<typename V::value_type>(op.data[i]):padding; return result; }
//----------------------------------------------------------------------------------------------

} /*math*/ } /*djah*/