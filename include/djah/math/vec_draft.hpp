#include <iostream>
#include <cmath>
#include <algorithm>
#include <functional>
using namespace std;

//----------------------------------------------------------------------------------------------
template<int N, typename T>
struct vector_base
{
	vector_base() { memset(data, 0, N*sizeof(T)); }
	T data[N];
};
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
template<typename T>
struct vector_base<2,T>
{
	vector_base() { memset(data, 0, 2*sizeof(T)); }
	vector_base(T _x, T _y) : x(_x), y(_y) {}

	union
	{
		struct { T x, y; };
		T data[2];
	};
};
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
template<typename T>
struct vector_base<3,T>
{
	vector_base() { memset(data, 0, 3*sizeof(T)); }
	vector_base(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

	union
	{
		struct { T x, y, z; };
		T data[3];
	};
};
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
template<typename T>
struct vector_base<4,T>
{
	vector_base() { memset(data, 0, 4*sizeof(T)); }
	vector_base(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}

	union
	{
		struct { T x, y, z, w; };
		T data[4];
	};
};
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
template<int N, typename T>
struct vector : public vector_base<N,T>
{
	static const int size = N;
	typedef T value_type;

	vector() {}
	vector(T _x, T _y) : vector_base(_x, _y) {}
	vector(T _x, T _y, T _z) : vector_base(_x, _y, _z) {}
	vector(T _w, T _x, T _y, T _z) : vector_base(_w, _x, _y, _z) {}

	vector(T (&array)[N]) { memcpy(data, array, N*sizeof(T)); }

	vector<N,T>& operator =(T (&array)[N]) { memcpy(data, array, N*sizeof(T)); return (*this); }

	template<typename F>	struct type			{ typedef float		float_t; };
	template<>				struct type<double> { typedef double	float_t; };
	typedef typename type<T>::float_t float_t;

	T				lengthSq()		const	{ return (*this) * (*this); }
	float_t			length()		const	{ return sqrt(static_cast<float_t>(lengthSq())); }
	vector<N,T>&	normalize()				{ return (*this) /= length(); }
	vector<N,T>		getNormalized()	const	{ return vector<N,T>(*this).normalize(); }

	const vector<N,T> operator -() const { vector<N,T> result; for(int i = 0; i < N; ++i) result.data[i] = -data[i]; return result; }

	vector<N,T>& operator +=(const vector<N,T> &rhs) { for(int i = 0; i < N; ++i) data[i] += rhs.data[i]; return (*this); }
	vector<N,T>& operator -=(const vector<N,T> &rhs) { for(int i = 0; i < N; ++i) data[i] -= rhs.data[i]; return (*this); }
	vector<N,T>& operator *=(T rhs) { for(int i = 0; i < N; ++i) data[i] *= rhs; return (*this); }
	vector<N,T>& operator /=(T rhs) { for(int i = 0; i < N; ++i) data[i] /= rhs; return (*this); }

	static const vector<N,T> null_vector;
};
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
template<typename T, int N>
bool operator ==(const vector<N,T> &lhs, const vector<N,T> &rhs)
{
	return std::equal(lhs.data, lhs.data+N, rhs.data);
}

template<typename T, int N>
bool operator !=(const vector<N,T> &lhs, const vector<N,T> &rhs)
{
	return !(lhs == rhs);
}

template<typename T, int N>
const vector<N,T> operator +(const vector<N,T> &lhs, const vector<N,T> &rhs)
{
	return vector<N,T>(lhs) += rhs;
}

template<typename T, int N>
const vector<N,T> operator -(const vector<N,T> &lhs, const vector<N,T> &rhs)
{
	return vector<N,T>(lhs) -= rhs;
}

template<typename T, int N>
const vector<N,T> operator *(const vector<N,T> &lhs, T rhs)
{
	return vector<N,T>(lhs) *= rhs;
}

template<typename T, int N>
const vector<N,T> operator *(T lhs, const vector<N,T> &rhs)
{
	return rhs * lhs;
}

template<typename T, int N>
const vector<N,T> operator /(const vector<N,T> &lhs, T rhs)
{
	return vector<N,T>(lhs) /= rhs;
}

template<typename T, int N>
const vector<N,T> dot(const vector<N,T> &lhs, const vector<N,T> &rhs)
{
	T result(0);
	for(int i = 0; i < N; ++i)
		result += lhs.data[i] * rhs.data[i];
	return result;
}

template<typename T, int N>
const vector<N,T> operator *(const vector<N,T> &lhs, const vector<N,T> &rhs)
{
	return dot(lhs, rhs);
}

template<typename T>
vector<3,T> cross(const vector<3,T> &lhs, const vector<3,T> &rhs)
{
	return vector<3,T>
	(
		lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.z * rhs.x - lhs.x * rhs.z,
		lhs.x * rhs.y - lhs.y * rhs.x
	);
}

template<typename T, int N>
const vector<N,T> operator ^(const vector<N,T> &lhs, const vector<N,T> &rhs)
{
	return cross(lhs, rhs);
}
//----------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------
template<typename U, int N, typename T>
const vector<N,U> cast(const vector<N,T> &op)
{ vector<N,U> result; for(int i = 0; i < N; ++i) result.data[i] = static_cast<U>(op.data[i]); return result; }

template<int N2, int N, typename T>
const vector<N2,T> resize(const vector<N,T> &op) 
{ vector<N2,T> result; for(int i = 0; i < N2; ++i) result.data[i] = i<N ? op.data[i]:0; return result; }

template<typename V, int N, typename T>
const V vector_cast(const vector<N,T> &op) 
{ V result; for(int i = 0; i < V::size; ++i) result.data[i] = i<N ? static_cast<typename V::value_type>(op.data[i]):0; return result; }
//----------------------------------------------------------------------------------------------
