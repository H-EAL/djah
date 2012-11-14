#ifndef DJAH_SYSTEM_INPUT_GAMEPAD_HPP
#define DJAH_SYSTEM_INPUT_GAMEPAD_HPP

#include <string>
#include <vector>
#include "axis.hpp"
#include "button.hpp"

namespace djah { namespace system { namespace input {

	enum ePovDirection
	{
		ePD_Neutral = -1,
		ePD_Down,
		ePD_Up,
		ePD_Left,
		ePD_Right
	};

	enum eXbox360Buttons
	{
		eX360_A,
		eX360_B,
		eX360_X,
		eX360_Y,
		eX360_LB,
		eX360_RB,
		eX360_Select,
		eX360_Start,
	};

	enum eXbox360Axis
	{
		eX360_LeftX,
		eX360_LeftY,
		eX360_LeftRightTrigger,
		eX360_RightX,
		eX360_RightY,
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
		
		void vibrate(int val);
		void vibrate(int left, int right);

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