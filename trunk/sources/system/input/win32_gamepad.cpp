#include "system/input/gamepad.hpp"
#include "platform.hpp"
#include "types.hpp"
#include <sstream>
#include <iostream>



/*
	USE THIS SOMEWHERE ELSE TO CREATE GAMEPADS, PROBABLY IN THE DEVICE CLASS
*/
/*
u32 nbGamePads = joyGetNumDevs();
std::cout << "Supporting " << nbGamePads << " gamepads" << std::endl;
		
JOYINFO ji;
for(u32 i = 0; i < nbGamePads; ++i)
	std::cout << "[" << i << "] " << (joyGetPos(i, &ji) == JOYERR_NOERROR) << std::endl;
*/

namespace djah { namespace system { namespace input {

	//----------------------------------------------------------------------------------------------
	gamepad::gamepad(unsigned int id)
		: id_(id)
		, plugged_(false)
		, nb_buttons_down_(0)
	{
		init();
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	gamepad::~gamepad()
	{
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	void gamepad::init()
	{
		JOYCAPS jc;

		MMRESULT result = joyGetDevCaps( id_, &jc, sizeof(JOYCAPS) );
		if( result == JOYERR_NOERROR )
		{
			name_ = jc.szPname;

			// Buttons
			initButtons( jc.wNumButtons );

			// Point Of View hat
			initPOV();
			
			// Axis
			static const char* axis_names[] =
			{
				"X", "Y",
				"Z", "R",
				"U", "V",
			};
			const unsigned int min_values[] =
			{
				jc.wXmin, jc.wYmin,
				jc.wZmin, jc.wRmin,
				jc.wUmin, jc.wVmin,
			};
			const unsigned int max_values[] =
			{
				jc.wXmax, jc.wYmax,
				jc.wZmax, jc.wRmax,
				jc.wUmax, jc.wVmax,
			};

			const u32 nb_axis = jc.wNumAxes > 6 ? 6 : jc.wNumAxes;
			axis_.reserve(nb_axis);
			for(u32 a = 0; a < nb_axis; ++a)
			{
				std::string axis_name = "GAMEPAD_AXIS_";
				axis_name += axis_names[a];
				axis_.push_back( axis(axis_name, a, min_values[a], max_values[a]) );
			}
		}
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	void gamepad::initButtons(unsigned int nb_buttons)
	{
		nb_buttons = nb_buttons > 32 ? 32 : nb_buttons;
		buttons_.reserve(nb_buttons);

		for(u32 b = 0; b < nb_buttons; ++b)
		{
			std::stringstream ss;
			ss << "GAMEPAD_BTN_" << (b+1);
			buttons_.push_back( button(ss.str(), (1 << b)) );
		}
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	void gamepad::update()
	{
		JOYINFOEX jie;
		jie.dwSize = sizeof(JOYINFOEX);

		plugged_ = joyGetPosEx(id_, &jie) == JOYERR_NOERROR;
		if( plugged_ )
		{
			// Buttons
			nb_buttons_down_ = jie.dwButtonNumber;
			u32 nb_buttons = buttons_.size();
			for(u32 b = 0; b < nb_buttons; ++b)
			{
				buttons_[b].setState( (jie.dwButtons & buttons_[b].id()) != 0 );
				//std::cout << buttons_[b].isDown() << std::endl;
			}

			// POV
			//std::cout << jie.dwPOV << std::endl;

			// Axis
			const int axis_values[] = 
			{
				jie.dwXpos, jie.dwYpos,
				jie.dwZpos, jie.dwRpos,
				jie.dwUpos, jie.dwVpos,
			};
			u32 nb_axis = axis_.size();
			for(u32 a = 0; a < nb_axis; ++a)
			{
				axis_[a].setValue((float)axis_values[a]);
			}
		}
	}
	//----------------------------------------------------------------------------------------------

} /*input*/ } /*system*/ } /*djah*/