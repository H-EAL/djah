#ifndef DJAH_UTILS_DETAIL_FUNCTOR_HANDLER_HPP
#define DJAH_UTILS_DETAIL_FUNCTOR_HANDLER_HPP

#include "functor_impl.hpp"

namespace djah { namespace utils { namespace detail {

	//----------------------------------------------------------------------------------------------
	template<typename ParentFunctor, typename Fun>
	class functor_handler
		: public functor_impl<typename ParentFunctor::ResultType, typename ParentFunctor::ParmList>
	{
	public:

		typedef typename ParentFunctor::ResultType ResultType;
		typedef typename ParentFunctor::Parm1 Parm1;
		typedef typename ParentFunctor::Parm2 Parm2;
		typedef typename ParentFunctor::Parm3 Parm3;
		typedef typename ParentFunctor::Parm4 Parm4;
		typedef typename ParentFunctor::Parm5 Parm5;
		typedef typename ParentFunctor::Parm6 Parm6;
		typedef typename ParentFunctor::Parm7 Parm7;
		typedef typename ParentFunctor::Parm8 Parm8;
		typedef typename ParentFunctor::Parm9 Parm9;

		functor_handler(const Fun &fun) : fun_(fun) {}
		functor_handler* clone() const { return new functor_handler(*this); }
		
		ResultType operator ()()
		{ return fun_(); }
		ResultType operator ()(Parm1 p1)
		{ return fun_(p1); }
		ResultType operator ()(Parm1 p1, Parm2 p2)
		{ return fun_(p1, p2); }
		ResultType operator ()(Parm1 p1, Parm2 p2, Parm3 p3)
		{ return fun_(p1, p2, p3); }
		ResultType operator ()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4)
		{ return fun_(p1, p2, p3, p4); }
		ResultType operator ()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5)
		{ return fun_(p1, p2, p3, p4, p5); }
		ResultType operator ()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5, Parm6 p6)
		{ return fun_(p1, p2, p3, p4, p5, p6); }
		ResultType operator ()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7)
		{ return fun_(p1, p2, p3, p4, p5, p6, p7); }
		ResultType operator ()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7, Parm8 p8)
		{ return fun_(p1, p2, p3, p4, p5, p6, p7, p8); }
		ResultType operator ()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9)
		{ return fun_(p1, p2, p3, p4, p5, p6, p7, p8, p9); }

	private:

		Fun fun_;
	};
	//----------------------------------------------------------------------------------------------

} /*detail*/ } /*utils*/ } /*djah*/

#endif /* DJAH_UTILS_DETAIL_FUNCTOR_HANDLER_HPP */