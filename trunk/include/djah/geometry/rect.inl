namespace djah { namespace geometry {

	//----------------------------------------------------------------------------------------------
	template<typename T>
	rect<T>::rect(T left, T top, T right, T bottom)
		: topLeft_(left, top)
		, bottomRight_(right, bottom)
	{

	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	rect<T>::rect(const math::vector2<T> &topLeft, const math::vector2<T> &bottomRight)
		: topLeft_(topLeft)
		, bottomRight_(bottomRight)
	{

	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	rect<T>::rect(const math::vector2<T> &topLeftPosition, T width, T height)
		: topLeft_(topLeftPosition)
		, bottomRight_(topLeftPosition.x() + width, topLeftPosition.y() + height)
	{

	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	T rect<T>::width() const
	{
		return std::abs(bottomRight_.x() - topLeft_.x());
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T>
	T rect<T>::height() const
	{
		return std::abs(bottomRight_.y() - topLeft_.y());
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	math::vector2<T>& rect<T>::topLeft()
	{
		return topLeft_;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T>
	math::vector2<T>& rect<T>::bottomRight()
	{
		return bottomRight_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	const math::vector2<T>& rect<T>::topLeft() const
	{
		return topLeft_;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T>
	const math::vector2<T>& rect<T>::bottomRight() const
	{
		return bottomRight_;
	}
	//----------------------------------------------------------------------------------------------

} /*geometry*/ } /*djah*/