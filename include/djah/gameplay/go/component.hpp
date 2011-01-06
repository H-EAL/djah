#ifndef DJAH_GAMEPLAY_GO_COMPONENT_HPP
#define DJAH_GAMEPLAY_GO_COMPONENT_HPP

#include "../../math.hpp"
#include "../../utils/murmur_hash.hpp"

namespace djah { namespace gameplay { namespace go {

	//----------------------------------------------------------------------------------------------
	template<typename T>
	class component_id;
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	class component
	{
	public:
		typedef component_id<T>			Id;
		typedef T						Component;
		typedef typename T::RealType	RealType; 

		component(const RealType &initialValue = RealType()) : data_(initialValue) {}

		RealType&		get()		{ return data_; }
		const RealType& get() const { return data_; }

	private:
		RealType data_;
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	#define MAKE_GOC_NAME_2(NAME)		NAME
	#define MAKE_GOC_NAME(NAME)			MAKE_GOC_NAME_2(NAME)_Component

	#define DJAH_MAKE_GOC(NAME, TYPE)	struct MAKE_GOC_NAME(NAME) {												\
											typedef TYPE RealType;													\
											static djah::u32 name_;													\
										};																			\
										u32 MAKE_GOC_NAME(NAME)::name_ = djah::utils::static_murmur_hash(#NAME);	\
										typedef djah::gameplay::go::component_id<MAKE_GOC_NAME(NAME)> NAME;
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

} /*go*/ } /*gameplay*/ } /*djah*/

#endif /* DJAH_GAMEPLAY_GO_COMPONENT_HPP */