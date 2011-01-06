#define C2(NAME)	NAME
#define C(NAME)		C2(NAME)_Component

//--------------------------------------------------------------------------------------------------
typedef TYPELIST_4 (

	C(Position),
	C(Velocity),
	C(Orientation),
	C(Speed)

) ComponentTypes;
//--------------------------------------------------------------------------------------------------

#undef C
#undef C2