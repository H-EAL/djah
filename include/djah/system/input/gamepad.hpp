#ifndef DJAH_SYSTEM_INPUT_GAMEPAD_HPP
#define DJAH_SYSTEM_INPUT_GAMEPAD_HPP

#include <string>
#include <vector>
#include "axis.hpp"
#include "button.hpp"

namespace djah { namespace system { namespace input {

	enum E_POV_DIRECTION
	{
		EPD_NEUTRAL = -1,
		EPD_DOWN,
		EPD_UP,
		EPD_LEFT,
		EPD_RIGHT
	};

	class gamepad
	{
	public:
		gamepad(unsigned int id);
		~gamepad();

		void			update();
		unsigned int	nbButtons()							const { return buttons_.size();		}
		unsigned int	nbButtonsDown()						const { return nb_buttons_down_;	}
		bool			isPlugged()							const { return plugged_;			}
		const button&	getButton(unsigned int button_id)	const { return buttons_[button_id]; }
		const axis&		getAxis(unsigned int axis_id)		const { return axis_[axis_id];		}

	private:
		void init();
		void initButtons(unsigned int nb_buttons);
		void initPOV() {}

	private:
		unsigned int		id_;
		std::string			name_;
		bool				plugged_;
		std::vector<axis>	axis_;
		std::vector<button> buttons_;
		//button				pov_[4];
		unsigned int		nb_buttons_down_;
	};

} /*input*/ } /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_INPUT_GAMEPAD_HPP */