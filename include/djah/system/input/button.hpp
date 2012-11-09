#ifndef DJAH_SYSTEM_INPUT_BUTTON_HPP
#define DJAH_SYSTEM_INPUT_BUTTON_HPP

#include <string>

namespace djah { namespace system { namespace input {

	class button
	{
	public:
		button(const std::string &name, unsigned int id) : name_(name), id_(id) {}
		~button() {}

		unsigned int		id()		const	{ return id_;					}
		const std::string&	name()		const	{ return name_;					}
		bool				isDown()	const	{ return pressed_;				}
		bool				pressed()	const	{ return pressed_ && toggled_;	}
		bool				isUp()		const	{ return !pressed_;				}
		bool				released()	const	{ return !pressed_ && toggled_; }

		void				setState(bool _pressed)
		{ toggled_ = (_pressed != pressed_); pressed_ = _pressed; }

	private:
		unsigned int	id_;
		std::string		name_;
		bool			pressed_;
		bool			toggled_;
	};

} /*input*/ } /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_INPUT_BUTTON_HPP */