namespace djah { namespace math {

	//----------------------------------------------------------------------------------------------
	template<typename T>
	transformation<T>::transformation
	(
		const vector<3,T> &tr = vector<3,T>::null_vector,
		const quaternion<T> &rot = quaternion<T>::identity,
		const vector<3,T> &s = vector<3,T>(T(1),T(1),T(1))
	)
	: scale_(s)
	, translation_(tr)
	, rotation_(rot)
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline const vector<3,T>& transformation<T>::scale() const
	{
		return scale_;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline const vector<3,T>& transformation<T>::translation() const
	{
		return translation_;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline const quaternion<T>& transformation<T>::rotation() const
	{
		return rotation_;
	}
	//----------------------------------------------------------------------------------------------

		
	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline void transformation<T>::setScale(const vector<3,T> &s)
	{
		scale_ = s;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline void transformation<T>::setScale(T s)
	{
		scale_.x = s;
		scale_.y = s;
		scale_.z = s;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline void transformation<T>::setTranslation(const vector<3,T> &tr)
	{
		translation_ = tr;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline void transformation<T>::setRotation(const quaternion<T> &rot)
	{
		rotation_ = rot;
	}
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline void transformation<T>::uniformScale(T s)
	{
		scale_ *= s;
	}
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline void transformation<T>::scale(const vector<3,T> &s)
	{
		scale_.x *= s.x;
		scale_.y *= s.y;
		scale_.z *= s.z;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline void transformation<T>::translate(const vector<3,T> &tr)
	{
		translation_ += tr;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline void transformation<T>::rotate(const quaternion<T> &rot)
	{
		rotation_ = rotation_ * rot;
	}
	//----------------------------------------------------------------------------------------------

		
	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline transformation<T>& transformation<T>::combine(const transformation<T> &other)
	{
		scale( other.scale_ );
		translate( other.translation_ );
		rotate( other.rotation_ );
	}
	//----------------------------------------------------------------------------------------------

		
	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline const matrix<4,4,T> transformation<T>::toMatrix4() const
	{
		return quat_to_matrix4(rotation_) * make_scale(scale_) * make_translation(translation_);
	}
	//----------------------------------------------------------------------------------------------

} /*math*/ } /*djah*/