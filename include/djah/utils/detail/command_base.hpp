#ifndef DJAH_UTILS_DETAIL_COMMAND_BASE_HPP
#define DJAH_UTILS_DETAIL_COMMAND_BASE_HPP

#include "../string_utils.hpp"
#include "../functor.hpp"

namespace djah { namespace utils { namespace detail {

	class command_base
	{
	public:

		virtual ~command_base() {};

		virtual std::string execute(const std::string &params) = 0;
	};


	//----------------------------------------------------------------------------------------------
	template<typename T> struct base_type { typedef T Type; };
	template<typename T> struct base_type<T&> { typedef T Type; };
	template<typename T> struct base_type<const T> { typedef T Type; };
	template<typename T> struct base_type<const T&> { typedef T Type; };
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	#define IMPLEMENT_FUNCTOR_CTORS(C)										\
	template<typename Fun>													\
	C(const Fun &fun) : functor(fun) {}										\
	template<typename PtrToObj, typename PtrToMemFn>						\
	C(const PtrToObj &pObj, PtrToMemFn pMemFn)	: functor(pObj, pMemFn) {}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename R>
	struct caller
	{
		template<typename FuncType>
		static std::string exec(FuncType &fun) { return param_to_string(fun()); }
		template<typename FuncType, typename P1>
		static std::string exec(FuncType &fun, P1 &p1) { return param_to_string(fun(p1)); }
		template<typename FuncType, typename P1, typename P2>
		static std::string exec(FuncType &fun, P1 &p1, P2 &p2) { return param_to_string(fun(p1, p2)); }
		template<typename FuncType, typename P1, typename P2, typename P3>
		static std::string exec(FuncType &fun, P1 &p1, P2 &p2, P3 &p3) { return param_to_string(fun(p1, p2, p3)); }
		template<typename FuncType, typename P1, typename P2, typename P3, typename P4>
		static std::string exec(FuncType &fun, P1 &p1, P2 &p2, P3 &p3, P4 &p4) { return param_to_string(fun(p1, p2, p3, p4)); }
		template<typename FuncType, typename P1, typename P2, typename P3, typename P4, typename P5>
		static std::string exec(FuncType &fun, P1 &p1, P2 &p2, P3 &p3, P4 &p4, P5 &p5) { return param_to_string(fun(p1, p2, p3, p4, p5)); }
	};
	//----------------------------------------------------------------------------------------------
	template<>
	struct caller<void>
	{
		template<typename FuncType>
		static std::string exec(FuncType &fun) { fun(); return ""; }
		template<typename FuncType, typename P1>
		static std::string exec(FuncType &fun, P1 &p1) { fun(p1); return ""; }
		template<typename FuncType, typename P1, typename P2>
		static std::string exec(FuncType &fun, P1 &p1, P2 &p2) { fun(p1, p2); return ""; }
		template<typename FuncType, typename P1, typename P2, typename P3>
		static std::string exec(FuncType &fun, P1 &p1, P2 &p2, P3 &p3) { fun(p1, p2, p3); return ""; }
		template<typename FuncType, typename P1, typename P2, typename P3, typename P4>
		static std::string exec(FuncType &fun, P1 &p1, P2 &p2, P3 &p3, P4 &p4) { fun(p1, p2, p3, p4); return ""; }
		template<typename FuncType, typename P1, typename P2, typename P3, typename P4, typename P5>
		static std::string exec(FuncType &fun, P1 &p1, P2 &p2, P3 &p3, P4 &p4, P5 &p5) { fun(p1, p2, p3, p4, p5); return ""; }
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename R>
	class command0
		: public command_base
		, public functor<R>
	{
	public:

		IMPLEMENT_FUNCTOR_CTORS(command0)

		virtual std::string execute(const std::string &params)
		{
			return caller<R>::exec(*this);
		}
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename R, typename P1>
	class command1
		: public command_base
		, public functor<R, TYPELIST_1(P1)>
	{
	public:

		IMPLEMENT_FUNCTOR_CTORS(command1);

		virtual std::string execute(const std::string &params)
		{
			typename base_type<P1>::Type p1;
			extract_params_from_string(params, p1);
			return caller<R>::exec(*this, p1);
		}
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename R, typename P1, typename P2>
	class command2
		: public command_base
		, public functor<R, TYPELIST_2(P1,P2)>
	{
	public:

		IMPLEMENT_FUNCTOR_CTORS(command2)

		virtual std::string execute(const std::string &params)
		{
			typename base_type<P1>::Type p1;
			typename base_type<P2>::Type p2;
			extract_params_from_string(params, p1, p2);
			return caller<R>::exec(*this, p1, p2);
		}
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename R, typename P1, typename P2, typename P3>
	class command3
		: public command_base
		, public functor<R, TYPELIST_3(P1,P2,P3)>
	{
	public:

		IMPLEMENT_FUNCTOR_CTORS(command3)

		virtual std::string execute(const std::string &params)
		{
			typename base_type<P1>::Type p1;
			typename base_type<P2>::Type p2;
			typename base_type<P3>::Type p3;
			extract_params_from_string(params, p1, p2, p3);
			return caller<R>::exec(*this, p1, p2, p3);
		}
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename R, typename P1, typename P2, typename P3, typename P4>
	class command4
		: public command_base
		, public functor<R, TYPELIST_4(P1,P2,P3,P4)>
	{
	public:

		IMPLEMENT_FUNCTOR_CTORS(command4)

			virtual std::string execute(const std::string &params)
		{
			typename base_type<P1>::Type p1;
			typename base_type<P2>::Type p2;
			typename base_type<P3>::Type p3;
			typename base_type<P4>::Type p4;
			extract_params_from_string(params, p1, p2, p3, p4);
			return caller<R>::exec(*this, p1, p2, p3, p4);
		}
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5>
	class command5
		: public command_base
		, public functor<R, TYPELIST_5(P1,P2,P3,P4,P5)>
	{
	public:

		IMPLEMENT_FUNCTOR_CTORS(command5)

			virtual std::string execute(const std::string &params)
		{
			typename base_type<P1>::Type p1;
			typename base_type<P2>::Type p2;
			typename base_type<P3>::Type p3;
			typename base_type<P4>::Type p4;
			typename base_type<P5>::Type p5;
			extract_params_from_string(params, p1, p2, p3, p4, p5);
			return caller<R>::exec(*this, p1, p2, p3, p4, p5);
		}
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	#undef IMPLEMENT_FUNCTOR_CTORS
	//----------------------------------------------------------------------------------------------

} /*detail*/ } /*utils*/ } /*djah*/

#endif /* DJAH_UTILS_DETAIL_COMMAND_BASE_HPP */