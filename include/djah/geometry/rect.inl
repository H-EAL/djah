namespace djah { namespace geometry {

	//----------------------------------------------------------------------------------------------
	template<typename T>
	rect<T>::rect(T left, T top, T right, T bottom)
		: top_left_(left, top)
		, bottom_right_(right, bottom)
	{

	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	rect<T>::rect(const math::vector<2,T> &topLeft, const math::vector<2,T> &bottomRight)
		: top_left_(topLeft)
		, bottom_right_(bottomRight)
	{

	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	rect<T>::rect(const math::vector<2,T> &topLeft, T width, T height)
		: top_left_(topLeft)
		, bottom_right_(topLeft.x + width, topLeft.y + height)
	{

	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline void rect<T>::repair()
	{
		if( top_left_.x > bottom_right_.x )
			std::swap(top_left_.x, bottom_right_.x);
		if( top_left_.y > bottom_right_.y )
			std::swap(top_left_.y, bottom_right_.y);
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline void rect<T>::invalidate()
	{
		top_left_.x = top_left_.y = std::numeric_limits<T>::max();
		bottom_right_.x = bottom_right_.y = std::numeric_limits<T>::min();
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline bool rect<T>::isValid() const
	{
		return (top_left_.x < bottom_right_.x) && (top_left_.y < bottom_right_.y);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline T rect<T>::width() const
	{
		return std::abs(bottom_right_.x - top_left_.x);
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline T rect<T>::height() const
	{
		return std::abs(bottom_right_.y - top_left_.y);
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline math::vector<2,T> rect<T>::extent() const
	{
		return math::create_vector(top_left_, bottom_right_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline math::vector<2,T>& rect<T>::topLeft()
	{
		return top_left_;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline math::vector<2,T>& rect<T>::bottomRight()
	{
		return bottom_right_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline const math::vector<2,T>& rect<T>::topLeft() const
	{
		return top_left_;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline const math::vector<2,T>& rect<T>::bottomRight() const
	{
		return bottom_right_;
	}
	//----------------------------------------------------------------------------------------------

} /*geometry*/ } /*djah*/