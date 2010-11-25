#ifndef DJAH_MATH_VECTOR4_HPP
#define DJAH_MATH_VECTOR4_HPP

#include "basic_vector.hpp"

namespace djah { namespace math {
	
	template<typename T>
	class vector4 : public basic_vector<4,T>
	{
	public:

		vector4(const T (&array)[size_])
			: basic_vector(array)
		{}

		explicit vector4(T x = T(0), T y = T(0), T z = T(0), T w = T(1))
		{
			data_[0] = x;
			data_[1] = y;
			data_[2] = z;
			data_[3] = w;
		}

		vector4(const vector_base<size_,T> &v)
		{
			std::copy(v.begin(), v.end(), data_);
		}

		// Vector members accessors
		T& x() { return data_[0]; }
		T& y() { return data_[1]; }
		T& z() { return data_[2]; }
		T& w() { return data_[3]; }

		// Const vector member accessors
		const T& x() const { return data_[0]; }
		const T& y() const { return data_[1]; }
		const T& z() const { return data_[2]; }
		const T& w() const { return data_[3]; }

		// Useful constants
		static const vector4<T> x_axis;
		static const vector4<T> y_axis;
		static const vector4<T> z_axis;
	};

	template<typename T> const vector4<T> vector4<T>::x_axis(T(1), T(0), T(0));
	template<typename T> const vector4<T> vector4<T>::y_axis(T(0), T(1), T(0));
	template<typename T> const vector4<T> vector4<T>::z_axis(T(0), T(0), T(1));

	// Cross product
	template<typename T>
	inline vector4<T> cross(const vector4<T> &lhs, const vector4<T> &rhs)
	{
		return vector4<T>
		(
			lhs.y() * rhs.z() - lhs.z() * rhs.y(),
			lhs.z() * rhs.x() - lhs.x() * rhs.z(),
			lhs.x() * rhs.y() - lhs.y() * rhs.x(),
			T(1)
		);
	}


	// Useful type definitions
	typedef vector4<unsigned char>	vector4ub;
	typedef vector4<unsigned short>	vector4us;
	typedef vector4<unsigned int>	vector4ui;
	typedef vector4<char>			vector4b;
	typedef vector4<short>			vector4s;
	typedef vector4<int>			vector4i;
	typedef vector4<float>			vector4f;
	typedef vector4<double>			vector4d;

	typedef vector4ub				point4ub;
	typedef vector4us				point4us;
	typedef vector4ui				point4ui;
	typedef vector4b				point4b;
	typedef vector4s				point4s;
	typedef vector4i				point4i;
	typedef vector4f				point4f;
	typedef vector4d				point4d;


} /*math*/ } /*djah*/


#endif /* DJAH_MATH_VECTOR4_HPP */