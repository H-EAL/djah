#ifndef DJAH_UTILS_COMMAND_HPP
#define DJAH_UTILS_COMMAND_HPP

#include "detail/command_base.hpp"

namespace djah { namespace utils {

	class command
	{
	private:

		// Should be somehow reference counted
		typedef detail::command_base *command_ptr;

	public:

		command(command_ptr cmd = 0) : command_(cmd) {}

		std::string operator ()(const std::string &params) const
		{
			if(!command_)
				return "";

			return command_->execute(params);
		}

	private:

		command_ptr command_;
	};


	//----------------------------------------------------------------------------------------------
	// Function object (0 to 5 parameters)
	//----------------------------------------------------------------------------------------------
	template<typename R, typename C, typename O>
	command* make_functor(O &obj, R (C::*func)())
	{
		return new command(new detail::command0<R>(obj, func));
	}
	//----------------------------------------------------------------------------------------------
	template<typename R, typename C, typename O, typename P1>
	command* make_functor(O &obj, R (C::*func)(P1))
	{
		return new command(new detail::command1<R,P1>(obj, func));
	}
	//----------------------------------------------------------------------------------------------
	template<typename R, typename C, typename O, typename P1, typename P2>
	command* make_functor(O &obj, R (C::*func)(P1, P2))
	{
		return new command(new detail::command2<R,P1,P2>(obj, func));
	}
	//----------------------------------------------------------------------------------------------
	template<typename R, typename C, typename O, typename P1, typename P2, typename P3>
	command* make_functor(O &obj, R (C::*func)(P1, P2, P3))
	{
		return new command(new detail::command3<R,P1,P2,P3>(obj, func));
	}
	//----------------------------------------------------------------------------------------------
	template<typename R, typename C, typename O, typename P1, typename P2, typename P3, typename P4>
	command* make_functor(O &obj, R (C::*func)(P1, P2, P3, P4))
	{
		return new command(new detail::command4<R,P1,P2,P3,P4>(obj, func));
	}
	//----------------------------------------------------------------------------------------------
	template<typename R, typename C, typename O, typename P1, typename P2, typename P3, typename P4, typename P5>
	command* make_functor(O &obj, R (C::*func)(P1, P2, P3, P4, P5))
	{
		return new command(new detail::command5<R,P1,P2,P3,P4,P5>(obj, func));
	}
	//----------------------------------------------------------------------------------------------
	template<typename T>
	command* make_command(T obj)
	{
		DJAH_LOG_TODO("The pointer to this local functor should be reference counted");
		T *newObj = new T(obj);
		return make_functor(newObj, &T::operator ());
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	// Free functions (0 to 5 parameters)
	//----------------------------------------------------------------------------------------------
	template<typename R>
	command* make_command(R (*Func)())
	{
		return new command(new detail::command0<R>(Func));
	}
	//----------------------------------------------------------------------------------------------
	template<typename R, typename P1>
	command* make_command(R (*Func)(P1))
	{
		return new command(new detail::command1<R,P1>(Func));
	}
	//----------------------------------------------------------------------------------------------
	template<typename R, typename P1, typename P2>
	command* make_command(R (*Func)(P1, P2))
	{
		return new command(new detail::command2<R,P1,P2>(Func));
	}
	//----------------------------------------------------------------------------------------------
	template<typename R, typename P1, typename P2, typename P3>
	command* make_command(R (*Func)(P1, P2, P3))
	{
		return new command(new detail::command3<R,P1,P2,P3>(Func));
	}
	//----------------------------------------------------------------------------------------------
	template<typename R, typename P1, typename P2, typename P3, typename P4>
	command* make_command(R (*Func)(P1, P2, P3, P4))
	{
		return new command(new detail::command4<R,P1,P2,P3,P4>(Func));
	}
	//----------------------------------------------------------------------------------------------
	template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5>
	command* make_command(R (*Func)(P1, P2, P3, P4, P5))
	{
		return new command(new detail::command5<R,P1,P2,P3,P4,P5>(Func));
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	// Member functions (0 to 5 parameters)
	//----------------------------------------------------------------------------------------------
	template<typename R, typename C, typename O>
	command* make_command(O &obj, R (C::*func)())
	{
		return new command(new detail::command0<R>(obj, func));
	}
	//----------------------------------------------------------------------------------------------
	template<typename R, typename C, typename O, typename P1>
	command* make_command(O &obj, R (C::*func)(P1))
	{
		return new command(new detail::command1<R,P1>(obj, func));
	}
	//----------------------------------------------------------------------------------------------
	template<typename R, typename C, typename O, typename P1, typename P2>
	command* make_command(O &obj, R (C::*func)(P1, P2))
	{
		return new command(new detail::command2<R,P1,P2>(obj, func));
	}
	//----------------------------------------------------------------------------------------------
	template<typename R, typename C, typename O, typename P1, typename P2, typename P3>
	command* make_command(O &obj, R (C::*func)(P1, P2, P3))
	{
		return new command(new detail::command3<R,P1,P2,P3>(obj, func));
	}
	//----------------------------------------------------------------------------------------------
	template<typename R, typename C, typename O, typename P1, typename P2, typename P3, typename P4>
	command* make_command(O &obj, R (C::*func)(P1, P2, P3, P4))
	{
		return new command(new detail::command4<R,P1,P2,P3,P4>(obj, func));
	}
	//----------------------------------------------------------------------------------------------
	template<typename R, typename C, typename O, typename P1, typename P2, typename P3, typename P4, typename P5>
	command* make_command(O &obj, R (C::*func)(P1, P2, P3, P4, P5))
	{
		return new command(new detail::command5<R,P1,P2,P3,P4,P5>(obj, func));
	}
	//----------------------------------------------------------------------------------------------

} /*utils*/ } /*djah*/

#endif /* DJAH_UTILS_COMMAND_HPP */