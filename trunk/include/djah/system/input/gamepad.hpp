#ifndef DJAH_SYSTEM_INPUT_GAMEPAD_HPP
#define DJAH_SYSTEM_INPUT_GAMEPAD_HPP

#include <string>
#include <vector>
#include "axis.hpp"
#include "button.hpp"
#include "trigger.hpp"

namespace djah { namespace system { namespace input {

	enum eGamepads
	{
		eGP_Gamepad_0 = 0,
		eGP_Gamepad_1,
		eGP_Gamepad_2,
		eGP_Gamepad_3
	};

	enum eXbox360Buttons
	{
		eX360_A,
		eX360_B,
		eX360_X,
		eX360_Y,
		eX360_LB,
		eX360_RighThumb,
		eX360_LeftThumb,
		eX360_RB,
		eX360_Back,
		eX360_Start,
		eX360_DPad_Down,
		eX360_DPad_Up,
		eX360_DPad_Left,
		eX360_DPad_Right,

		eX360_ButtonsCount
	};

	enum eXbox360Axis
	{
		eX360_LeftX,
		eX360_LeftY,
		eX360_RightX,
		eX360_RightY,

		eX360_AxisCount
	};

	enum eX360_Triggers
	{
		eX360_LeftTrigger,
		eX360_RightTrigger,

		eX360_TriggersCount
	};

	class gamepad
	{
	public:
		gamepad(unsigned int id);
		~gamepad();

		void			update();
		unsigned int	nbButtons()							const { return buttons_.size();			}
		bool			isPlugged()							const { return plugged_;				}
		const button&	getButton(unsigned int button_id)	const { return buttons_[button_id];		}
		const axis&		getAxis(unsigned int axis_id)		const { return axis_[axis_id];			}
		const trigger&	getTrigger(unsigned int trigger_id)	const { return triggers_[trigger_id];	}
		
		void vibrate(float val);
		void vibrate(float left, float right);

	private:
		void init();

	private:
		unsigned int		 id_;
		std::string			 name_;
		bool				 plugged_;
        bool                 initialized_;
		std::vector<axis>	 axis_;
		std::vector<button>	 buttons_;
		std::vector<trigger> triggers_;
	};

} /*input*/ } /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_INPUT_GAMEPAD_HPP */