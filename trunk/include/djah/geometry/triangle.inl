namespace djah { namespace geometry {

	//-----------------------------------------------------------------------------
	template<typename T>
	triangle<T>::triangle(const math::vector3<T> &a, const math::vector3<T> &b, const math::vector3<T> &c)
		: a_(a)
		, b_(b)
		, c_(c)
	{
	}
	//-----------------------------------------------------------------------------

	
	//-----------------------------------------------------------------------------
	template<typename T>
	math::vector3<T>&		triangle<T>::a()        { return a_; }
	template<typename T>
	math::vector3<T>&		triangle<T>::b()        { return b_; }
	template<typename T>
	math::vector3<T>&		triangle<T>::c()        { return c_; }
	//-----------------------------------------------------------------------------


	//-----------------------------------------------------------------------------
	template<typename T>
	const math::vector3<T>&	triangle<T>::a() const	{ return a_; }
	template<typename T>
	const math::vector3<T>&	triangle<T>::b() const	{ return b_; }
	template<typename T>
	const math::vector3<T>&	triangle<T>::c() const	{ return c_; }
	//-----------------------------------------------------------------------------

} /*geometry*/ } /*djah*/