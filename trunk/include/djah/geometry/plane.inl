namespace djah { namespace geometry {

	//-----------------------------------------------------------------------------
	template<typename T>
	plane<T>::plane(T distance, const math::vector<3,T> &normal)
		: distance_(distance)
		, normal_(normal)
	{
	}
	//-----------------------------------------------------------------------------
	
	
	//-----------------------------------------------------------------------------
	template<typename T>
	plane<T>::plane(T a, T b, T c, T d)
		: distance_(d)
		, normal_(a,b,c)
	{
	}
	//-----------------------------------------------------------------------------
	
	
	//-----------------------------------------------------------------------------
	template<typename T>
	T&                plane<T>::distance() { return distance_;   }
	template<typename T>
	math::vector<3,T>& plane<T>::normal()  { return normal_;     }
	template<typename T>
	T&                plane<T>::a()        { return normal_.x; }
	template<typename T>
	T&                plane<T>::b()        { return normal_.y; }
	template<typename T>
	T&                plane<T>::c()        { return normal_.z; }
	template<typename T>
	T&                plane<T>::d()        { return distance_;   }
	//-----------------------------------------------------------------------------


	//-----------------------------------------------------------------------------
	template<typename T>
	const T&				 plane<T>::distance() const { return distance_;   }
	template<typename T>
	const math::vector<3,T>& plane<T>::normal()   const { return normal_;     }
	template<typename T>
	const T&                 plane<T>::a()        const { return normal_.x; }
	template<typename T>
	const T&                 plane<T>::b()        const { return normal_.y; }
	template<typename T>
	const T&                 plane<T>::c()        const { return normal_.z; }
	template<typename T>
	const T&                 plane<T>::d()        const { return distance_;   }
	//-----------------------------------------------------------------------------


	//-----------------------------------------------------------------------------
	template<typename T>
	T plane<T>::solve(const math::vector<3,T> &point) const
	{
		return point * normal_ + distance_;
	}
	//-----------------------------------------------------------------------------
	
	
	//-----------------------------------------------------------------------------
	template<typename T>
	int plane<T>::relativePosition(const math::vector<3,T> &point) const
	{
		const T eq = solve(point);
		return  eq == T(0) ? ON_PLANE : (eq < T(0) ? BEHIND_PLANE : IN_FRONT_OF_PLANE);
	}
	//-----------------------------------------------------------------------------
	
	
	//-----------------------------------------------------------------------------
	template<typename T>
	bool plane<T>::isOn(const math::vector<3,T> &point) const
	{
		return relativePosition(point) == ON_PLANE;
	}
	//-----------------------------------------------------------------------------


	//-----------------------------------------------------------------------------
	template<typename T>
	bool plane<T>::isBehind(const math::vector<3,T> &point) const
	{
		return relativePosition(point) == BEHIND_PLANE;
	}
	//-----------------------------------------------------------------------------


	//-----------------------------------------------------------------------------
	template<typename T>
	bool plane<T>::isInFrontOf(const math::vector<3,T> &point) const
	{
		return relativePosition(point) == IN_FRONT_OF_PLANE;
	}
	//-----------------------------------------------------------------------------

} /*geometry*/ } /*djah*/