#ifndef DJAH_TYPELIST_HPP
#define DJAH_TYPELIST_HPP

#include "macros.hpp"

#define TYPELIST_1(T1) djah::typelist<T1, djah::nulltype>
#define TYPELIST_2(T1,T2) djah::typelist<T1, TYPELIST_1(T2) >
#define TYPELIST_3(T1,T2,T3) djah::typelist<T1, TYPELIST_2(T2,T3) >
#define TYPELIST_4(T1,T2,T3,T4) djah::typelist<T1, TYPELIST_3(T2,T3,T4) >
#define TYPELIST_5(T1,T2,T3,T4,T5) djah::typelist<T1, TYPELIST_4(T2,T3,T4,T5) >
#define TYPELIST_6(T1,T2,T3,T4,T5,T6) djah::typelist<T1, TYPELIST_5(T2,T3,T4,T5,T6) >
#define TYPELIST_7(T1,T2,T3,T4,T5,T6,T7) djah::typelist<T1, TYPELIST_6(T2,T3,T4,T5,T6,T7) >
#define TYPELIST_8(T1,T2,T3,T4,T5,T6,T7,T8) djah::typelist<T1, TYPELIST_7(T2,T3,T4,T5,T6,T7,T8) >
#define TYPELIST_9(T1,T2,T3,T4,T5,T6,T7,T8,T9) djah::typelist<T1, TYPELIST_8(T2,T3,T4,T5,T6,T7,T8,T9) >

#define TYPELIST(...)	VA_ARGS_MACRO(TYPELIST_, __VA_ARGS__)

namespace djah {

	//----------------------------------------------------------------------------------------------
	struct empty_type {};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	struct nulltype {};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename H, typename T>
	struct typelist
	{
		typedef H Head;
		typedef T Tail;
	};
	//----------------------------------------------------------------------------------------------


	namespace tl {
		
		//------------------------------------------------------------------------------------------
		// Compute the length of the typelist
		//------------------------------------------------------------------------------------------
		template<typename TL> struct length;
		//------------------------------------------------------------------------------------------
		template<>
		struct length<nulltype>
		{
			enum { value = 0 };
		};
		//------------------------------------------------------------------------------------------
		template<typename H, typename T>
		struct length< typelist<H,T> >
		{
			enum { value = 1 + length<T>::value };
		};
		//------------------------------------------------------------------------------------------


		//------------------------------------------------------------------------------------------
		// Return the type at the specified position in the typelist
		// A compilation time error will be raised if i is out of bound
		//------------------------------------------------------------------------------------------
		template<typename TL, unsigned int i> struct type_at;
		//------------------------------------------------------------------------------------------
		template<typename Head, typename Tail>
		struct type_at<typelist<Head,Tail>, 0>
		{
			typedef Head Result;
		};
		//------------------------------------------------------------------------------------------
		template<typename Head, typename Tail, unsigned int i>
		struct type_at<typelist<Head,Tail>, i>
		{
			typedef typename type_at<Tail,i-1>::Result Result;
		};
		//------------------------------------------------------------------------------------------


		//------------------------------------------------------------------------------------------
		// Return the type at the specified position in the typelist
		// If i is out of bound it returns the specified default type
		//------------------------------------------------------------------------------------------
		template<typename TL, unsigned int i, typename DefaultType> struct type_at_non_strict;
		//------------------------------------------------------------------------------------------
		template<unsigned int i, typename DefaultType>
		struct type_at_non_strict<nulltype, i, DefaultType>
		{
			typedef DefaultType Result;
		};
		//------------------------------------------------------------------------------------------
		template<typename Head, typename Tail, typename DefaultType>
		struct type_at_non_strict<typelist<Head,Tail>, 0, DefaultType>
		{
			typedef Head Result;
		};
		//------------------------------------------------------------------------------------------
		template<typename Head, typename Tail, unsigned int i, typename DefaultType>
		struct type_at_non_strict<typelist<Head,Tail>, i, DefaultType>
		{
			typedef typename type_at_non_strict<Tail,i-1,DefaultType>::Result Result;
		};
		//------------------------------------------------------------------------------------------


		//------------------------------------------------------------------------------------------
		// Append a typelist to an other
		//------------------------------------------------------------------------------------------
		template<typename TL, typename T> struct append;
		//------------------------------------------------------------------------------------------
		template<>
		struct append<nulltype,nulltype>
		{
			typedef nulltype Result;
		};
		//------------------------------------------------------------------------------------------
		template<typename T>
		struct append<nulltype, T>
		{
			typedef T Result;
		};
		//------------------------------------------------------------------------------------------
		template<typename Head, typename Tail>
		struct append<nulltype,typelist<Head,Tail> >
		{
			typedef typelist<Head,Tail> Result;
		};
		//------------------------------------------------------------------------------------------
		template<typename Head, typename Tail, typename T>
		struct append<typelist<Head,Tail>,T>
		{
			typedef typelist<Head,typename append<Tail,T>::Result> Result;
		};
		//------------------------------------------------------------------------------------------

	} /*tl*/

} /*djah*/

#endif /* DJAH_TYPELIST_HPP */