#ifndef DJAH_UTILS_FUNCTOR_HPP
#define DJAH_UTILS_FUNCTOR_HPP

#include "detail/functor_handler.hpp"
#include "detail/member_function_handler.hpp"

namespace djah { namespace utils {

	template<typename R = void, typename TL = nulltype>
	class functor
	{
	private:

		typedef detail::functor_impl<R,TL> Impl;

	public:

		typedef TL ParmList;
		typedef R ResultType;
		typedef typename tl::type_at_non_strict<ParmList,0,empty_type>::Result Parm1;
		typedef typename tl::type_at_non_strict<ParmList,1,empty_type>::Result Parm2;
		typedef typename tl::type_at_non_strict<ParmList,2,empty_type>::Result Parm3;
		typedef typename tl::type_at_non_strict<ParmList,3,empty_type>::Result Parm4;
		typedef typename tl::type_at_non_strict<ParmList,4,empty_type>::Result Parm5;
		typedef typename tl::type_at_non_strict<ParmList,5,empty_type>::Result Parm6;
		typedef typename tl::type_at_non_strict<ParmList,6,empty_type>::Result Parm7;
		typedef typename tl::type_at_non_strict<ParmList,7,empty_type>::Result Parm8;
		typedef typename tl::type_at_non_strict<ParmList,8,empty_type>::Result Parm9;

		functor() : impl_(0) {}
		functor(const functor &func) : impl_(func.impl_->clone()) {}
		functor& operator =(const functor &func) { impl_ = func.impl_->clone(); }
		explicit functor(std::auto_ptr<Impl> impl) : impl_(impl) {}

		template<typename Fun>
		functor(const Fun &fun)
			: impl_( new detail::functor_handler<functor, Fun>(fun) ) {}

		template<typename PtrToObj, typename PtrToMemFn>
		functor(const PtrToObj &pObj, PtrToMemFn pMemFn)
			: impl_( new detail::member_function_handler<functor, PtrToObj, PtrToMemFn>(pObj, pMemFn) ) {}

		ResultType operator ()()
		{ return (*impl_)(); }
		ResultType operator ()(Parm1 p1)
		{ return (*impl_)(p1); }
		ResultType operator ()(Parm1 p1, Parm2 p2)
		{ return (*impl_)(p1, p2); }
		ResultType operator ()(Parm1 p1, Parm2 p2, Parm3 p3)
		{ return (*impl_)(p1, p2, p3); }
		ResultType operator ()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4)
		{ return (*impl_)(p1, p2, p3, p4); }
		ResultType operator ()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5)
		{ return (*impl_)(p1, p2, p3, p4, p5); }
		ResultType operator ()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5, Parm6 p6)
		{ return (*impl_)(p1, p2, p3, p4, p5, p6); }
		ResultType operator ()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7)
		{ return (*impl_)(p1, p2, p3, p4, p5, p6, p7); }
		ResultType operator ()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7, Parm8 p8)
		{ return (*impl_)(p1, p2, p3, p4, p5, p6, p7, p8); }
		ResultType operator ()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9)
		{ return (*impl_)(p1, p2, p3, p4, p5, p6, p7, p8, p9); }

	private:

		std::auto_ptr<Impl> impl_;
	};

} /*utils*/ } /*djah*/

#endif /* DJAH_UTILS_FUNCTOR_HPP */