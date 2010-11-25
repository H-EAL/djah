#ifndef DJAH_MATH_VECTOR3_HPP
#define DJAH_MATH_VECTOR3_HPP

#include "basic_vector.hpp"

namespace djah { namespace math {

	template<typename T>
	class vector3 : public basic_vector<3,T>
	{
	public:

		vector3(const T (&array)[size_])
			: basic_vector(array)
		{}

		explicit vector3(T x = T(0), T y = T(0), T z = T(0))
		{
			data_[0] = x;
			data_[1] = y;
			data_[2] = z;
		}

		vector3(const vector_base<size_,T> &v)
		{
			std::copy(v.begin(), v.end(), data_);
		}

		// Vector members accessors
		T& x() { return data_[0]; }
		T& y() { return data_[1]; }
		T& z() { return data_[2]; }

		// Const vector member accessors
		const T& x() const { return data_[0]; }
		const T& y() const { return data_[1]; }
		const T& z() const { return data_[2]; }

		// Useful constants
		static const vector3<T> x_axis;
		static const vector3<T> y_axis;
		static const vector3<T> z_axis;
	};

	template<typename T> const vector3<T> vector3<T>::x_axis(T(1), T(0), T(0));
	template<typename T> const vector3<T> vector3<T>::y_axis(T(0), T(1), T(0));
	template<typename T> const vector3<T> vector3<T>::z_axis(T(0), T(0), T(1));


	// Cross product
	template<typename T>
	inline vector3<T> cross(const vector3<T> &lhs, const vector3<T> &rhs)
	{
		return vector3<T>
		(
			lhs.y() * rhs.z() - lhs.z() * rhs.y(),
			lhs.z() * rhs.x() - lhs.x() * rhs.z(),
			lhs.x() * rhs.y() - lhs.y() * rhs.x()
		);
	}


	// Useful type definitions
	typedef vector3<unsigned char>	vector3ub;
	typedef vector3<unsigned short>	vector3us;
	typedef vector3<unsigned int>	vector3ui;
	typedef vector3<char>			vector3b;
	typedef vector3<short>			vector3s;
	typedef vector3<int>			vector3i;
	typedef vector3<float>			vector3f;
	typedef vector3<double>			vector3d;

	typedef vector3ub				point3ub;
	typedef vector3us				point3us;
	typedef vector3ui				point3ui;
	typedef vector3b				point3b;
	typedef vector3s				point3s;
	typedef vector3i				point3i;
	typedef vector3f				point3f;
	typedef vector3d				point3d;


} /*math*/ } /*djah*/


#endif /* DJAH_MATH_VECTOR3_HPP */