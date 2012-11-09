#ifndef DJAH_UTILS_DETAIL_FUNCTOR_IMPL_HPP
#define DJAH_UTILS_DETAIL_FUNCTOR_IMPL_HPP

#include "../typelist.hpp"

namespace djah { namespace utils { namespace detail {

	//----------------------------------------------------------------------------------------------
	template<typename R, typename TL>
	class functor_impl;
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename R>
	class functor_impl<R,nulltype>
	{
	public:
		virtual R operator ()() = 0;
		virtual functor_impl* clone() const = 0;
		virtual ~functor_impl() {}
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename R, typename P1>
	class functor_impl<R, TYPELIST_1(P1)>
	{
	public:
		virtual R operator ()(P1) = 0;
		virtual functor_impl* clone() const = 0;
		virtual ~functor_impl() {}
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename R, typename P1, typename P2>
	class functor_impl<R, TYPELIST_2(P1, P2)>
	{
	public:
		virtual R operator ()(P1, P2) = 0;
		virtual functor_impl* clone() const = 0;
		virtual ~functor_impl() {}
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename R, typename P1, typename P2, typename P3>
	class functor_impl<R, TYPELIST_3(P1, P2, P3)>
	{
	public:
		virtual R operator ()(P1, P2, P3) = 0;
		virtual functor_impl* clone() const = 0;
		virtual ~functor_impl() {}
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename R, typename P1, typename P2, typename P3, typename P4>
	class functor_impl<R, TYPELIST_4(P1, P2, P3, P4)>
	{
	public:
		virtual R operator ()(P1, P2, P3, P4) = 0;
		virtual functor_impl* clone() const = 0;
		virtual ~functor_impl() {}
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5>
	class functor_impl<R, TYPELIST_5(P1, P2, P3, P4, P5)>
	{
	public:
		virtual R operator ()(P1, P2, P3, P4, P5) = 0;
		virtual functor_impl* clone() const = 0;
		virtual ~functor_impl() {}
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	class functor_impl<R, TYPELIST_6(P1, P2, P3, P4, P5, P6)>
	{
	public:
		virtual R operator ()(P1, P2, P3, P4, P5, P6) = 0;
		virtual functor_impl* clone() const = 0;
		virtual ~functor_impl() {}
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6,
		typename P7>
	class functor_impl<R, TYPELIST_7(P1, P2, P3, P4, P5, P6, P7)>
	{
	public:
		virtual R operator ()(P1, P2, P3, P4, P5, P6, P7) = 0;
		virtual functor_impl* clone() const = 0;
		virtual ~functor_impl() {}
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6,
		typename P7, typename P8>
	class functor_impl<R, TYPELIST_8(P1, P2, P3, P4, P5, P6, P7, P8)>
	{
	public:
		virtual R operator ()(P1, P2, P3, P4, P5, P6, P7, P8) = 0;
		virtual functor_impl* clone() const = 0;
		virtual ~functor_impl() {}
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6,
		typename P7, typename P8, typename P9>
	class functor_impl<R, TYPELIST_9(P1, P2, P3, P4, P5, P6, P7, P8, P9)>
	{
	public:
		virtual R operator ()(P1, P2, P3, P4, P5, P6, P7, P8, P9) = 0;
		virtual functor_impl* clone() const = 0;
		virtual ~functor_impl() {}
	};
	//----------------------------------------------------------------------------------------------

} /*detail*/ } /*utils*/ } /*djah*/

#endif /* DJAH_UTILS_DETAIL_FUNCTOR_IMPL_HPP */