#ifndef DJAH_GAMEPLAY_GAME_OBJECT_COMPONENT_HPP
#define DJAH_GAMEPLAY_GAME_OBJECT_COMPONENT_HPP

#include "../math.hpp"
#include "../utils/murmur_hash.hpp"

namespace djah { namespace gameplay {

	template<typename T>
	class game_object_component_id;

	//----------------------------------------------------------------------------------------------
	template<typename T>
	class game_object_component
	{
	public:
		typedef game_object_component_id<T> Id;
		typedef T							Component;
		typedef typename T::InnerType		Type;

		game_object_component(const Type &initialValue = Type()) : data_(initialValue) {}

		Type&		get()		{ return data_; }
		const Type& get() const { return data_; }

	private:
		Type data_;
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	#define MAKE_GOC_NAME_2(NAME)		NAME
	#define MAKE_GOC_NAME(NAME)			MAKE_GOC_NAME_2(NAME)_Component
	#define DJAH_MAKE_GOC(NAME, TYPE)	struct MAKE_GOC_NAME(NAME) {								\
											typedef TYPE InnerType;									\
										};															\
										typedef game_object_component_id<MAKE_GOC_NAME(NAME)> NAME;
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	DJAH_MAKE_GOC(Position,		djah::math::vector3f);
	DJAH_MAKE_GOC(Orientation,	djah::math::quatf);
	DJAH_MAKE_GOC(Velocity,		djah::math::vector3f);
	DJAH_MAKE_GOC(Speed,		djah::u16);
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	#undef MAKE_GOC_NAME_2
	#undef MAKE_GOC_NAME
	//----------------------------------------------------------------------------------------------

} /*gameplay*/ } /*djah*/

#endif /* DJAH_GAMEPLAY_GAME_OBJECT_COMPONENT_HPP */