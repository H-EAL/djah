#ifndef DJAH_MATH_TRANSFORM_HPP
#define DJAH_MATH_TRANSFORM_HPP

#include "vector3.hpp"
#include "quaternion.hpp"
#include "matrix4.hpp"
#include "math_utils.hpp"

namespace djah { namespace math { 

	//----------------------------------------------------------------------------------------------
	template<typename T>
	class transform
	{
	public:
		transform
		(
			const vector<3,T> &tr = vector<3,T>::null_vector,
			const quaternion<T> &rot = quaternion<T>::identity,
			const vector<3,T> &s = vector<3,T>(T(1),T(1),T(1))
		);

		// Getters
		const vector<3,T>&		scale()			const;
		const vector<3,T>&		translation()	const;
		const quaternion<T>&	rotation()		const;

		// Setters
		void setScale(const vector<3,T> &s);
		void setScale(T s);
		void setTranslation(const vector<3,T> &tr);
		void setRotation(const quaternion<T> &rot);

		// Transformers \o/
		void uniformScale(T s);
		void scale(const vector<3,T> &s);
		void translate(const vector<3,T> &tr);
		void rotate(const quaternion<T> &rot);

		// Combine with another transformation and keep the result
		transform<T>& combine(const transform<T> &other);

		// Convert to a more convenient matrix4
		const matrix<4,4,T> toMatrix4() const;

	private:
		vector<3,T>		translation_;
		vector<3,T>		scale_;
		quaternion<T>	rotation_;
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	// Combine two transformations
	//----------------------------------------------------------------------------------------------
	template<typename T>
	transform<T> combine_transforms(const transform<T> &lhs, const transform<T> &rhs);
	
	
	//----------------------------------------------------------------------------------------------
	// Useful type definitions
	typedef transform<float>	transform_f;
	typedef transform<double>	transform_d;

} /*math*/ } /*djah*/

#include "transform.inl"

#endif /* DJAH_MATH_TRANSFORM_HPP */