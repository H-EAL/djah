#ifndef DJAH_MATH_VECTOR2_HPP
#define DJAH_MATH_VECTOR2_HPP

#include "basic_vector.hpp"

namespace djah { namespace math {

	template<typename T>
	class vector2
		: public basic_vector<2,T>
	{
	public:

		vector2(const T (&array)[2])
			: basic_vector<2,T>(array)
		{}

		explicit vector2(T x = T(0), T y = T(0))
		{
			detail::vector_base<2,T>::data_[0] = x;
			detail::vector_base<2,T>::data_[1] = y;
		}

		vector2(const vector_base<2,T> &v)
		{
			std::copy(v.begin(), v.end(), detail::vector_base<2,T>::data_);
		}

		// Vector members accessors
		T& x() { return detail::vector_base<2,T>::data_[0]; }
		T& y() { return detail::vector_base<2,T>::data_[1]; }

		// Const vector member accessors
		const T& x() const { return detail::vector_base<2,T>::data_[0]; }
		const T& y() const { return detail::vector_base<2,T>::data_[1]; }
	};


	// Useful type definitions
	typedef vector2<unsigned char>	vector2ub;
	typedef vector2<unsigned short>	vector2us;
	typedef vector2<unsigned int>	vector2ui;
	typedef vector2<char>			vector2b;
	typedef vector2<short>			vector2s;
	typedef vector2<int>			vector2i;
	typedef vector2<float>			vector2f;
	typedef vector2<double>			vector2d;

	typedef vector2ub				point2ub;
	typedef vector2us				point2us;
	typedef vector2ui				point2ui;
	typedef vector2b				point2b;
	typedef vector2s				point2s;
	typedef vector2i				point2i;
	typedef vector2f				point2f;
	typedef vector2d				point2d;

} /*math*/ } /*djah*/

#endif /* DJAH_MATH_VECTOR2_HPP */