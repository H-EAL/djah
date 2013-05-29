template<typename T>
class radian;

template<typename T>
class degree
{
public:
	degree(T val) : value(val) {}
	degree(radian<T> rad) : value(math::rad_to_deg(rad.value)) {}

	operator radian<T>() { return radian<T>(*this); }

	T value;
};

template<typename T>
class radian
{
public:
	radian(T val) : value(val) {}
	radian(degree<T> deg) : value(math::deg_to_rad(deg.value)) {}

	operator degree<T>() { return degree<T>(*this); }

	T value;
};