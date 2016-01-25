#include "djah/system/input/gamepad.hpp"

#include <climits>

#include "djah/platform.hpp"
#include "djah/types.hpp"

namespace djah { namespace system { namespace input {

	//----------------------------------------------------------------------------------------------
	gamepad::gamepad(unsigned int id)
		: id_(id)
		, plugged_(false)
		, initialized_(false)
	{
		init();
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	gamepad::~gamepad()
	{
		if( plugged_ )
			vibrate(0.0f);
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	void gamepad::init()
	{
		// Clean the state
		XINPUT_STATE controllerState;
		memset(&controllerState, 0, sizeof(XINPUT_STATE));

		// Get the state
		DWORD result = XInputGetState(id_, &controllerState);
		plugged_ = ( result == ERROR_SUCCESS );
		if( !plugged_ )
			return;

		name_ = "X360_Controller_" + std::to_string(id_);

		// Buttons
		buttons_.reserve(eX360_ButtonsCount);
		buttons_.push_back( button("GAMEPAD_BTN_A", XINPUT_GAMEPAD_A) );
		buttons_.push_back( button("GAMEPAD_BTN_B", XINPUT_GAMEPAD_B) );
		buttons_.push_back( button("GAMEPAD_BTN_X", XINPUT_GAMEPAD_X) );
		buttons_.push_back( button("GAMEPAD_BTN_Y", XINPUT_GAMEPAD_Y) );
		buttons_.push_back( button("GAMEPAD_BTN_LB", XINPUT_GAMEPAD_LEFT_SHOULDER) );
		buttons_.push_back( button("GAMEPAD_BTN_RIGHT_THUMB", XINPUT_GAMEPAD_RIGHT_THUMB) );
		buttons_.push_back( button("GAMEPAD_BTN_LEFT_THUMB", XINPUT_GAMEPAD_LEFT_THUMB) );
		buttons_.push_back( button("GAMEPAD_BTN_RB", XINPUT_GAMEPAD_RIGHT_SHOULDER) );
		buttons_.push_back( button("GAMEPAD_BTN_BACK", XINPUT_GAMEPAD_BACK) );
		buttons_.push_back( button("GAMEPAD_BTN_START", XINPUT_GAMEPAD_START) );
		buttons_.push_back( button("GAMEPAD_BTN_DPAD_DOWN", XINPUT_GAMEPAD_DPAD_DOWN) );
		buttons_.push_back( button("GAMEPAD_BTN_DPAD_UP", XINPUT_GAMEPAD_DPAD_UP) );
		buttons_.push_back( button("GAMEPAD_BTN_DPAD_LEFT", XINPUT_GAMEPAD_DPAD_LEFT) );
		buttons_.push_back( button("GAMEPAD_BTN_DPAD_RIGHT", XINPUT_GAMEPAD_DPAD_RIGHT) );
			
		// Axis
		axis_.reserve(eX360_AxisCount);
		axis_.push_back( axis("GAMEPAD_AXIS_LX", eX360_LeftX,  SHRT_MIN, SHRT_MAX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)  );
		axis_.push_back( axis("GAMEPAD_AXIS_LY", eX360_LeftY,  SHRT_MIN, SHRT_MAX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)  );
		axis_.push_back( axis("GAMEPAD_AXIS_RX", eX360_RightX, SHRT_MIN, SHRT_MAX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) );
		axis_.push_back( axis("GAMEPAD_AXIS_RY", eX360_RightY, SHRT_MIN, SHRT_MAX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) );

		// Triggers
		triggers_.reserve(eX360_TriggersCount);
		triggers_.push_back( trigger("GAMEPAD_TRIGGER_LEFT",  eX360_LeftTrigger,  UCHAR_MAX, XINPUT_GAMEPAD_TRIGGER_THRESHOLD ) );
		triggers_.push_back( trigger("GAMEPAD_TRIGGER_RIGHT", eX360_RightTrigger, UCHAR_MAX, XINPUT_GAMEPAD_TRIGGER_THRESHOLD ) );

		initialized_ = true;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	void gamepad::update()
	{
		// Clean the state
		XINPUT_STATE controllerState;
		memset(&controllerState, 0, sizeof(XINPUT_STATE));

		// Get the state
		DWORD result = XInputGetState(id_, &controllerState);

		// Check if the gamepad is still plugged in
		plugged_ = (result == ERROR_SUCCESS);
		if( !plugged_ )
		{
			initialized_ = false;
			return;
		}
		
		if( !initialized_ )
		{
			init();
		}

		// Check if there's any change in the gamepad state
		static DWORD dwLastPacketNumber = 0;
		if( dwLastPacketNumber == controllerState.dwPacketNumber )
		{
			// No changes
			return;
		}
		dwLastPacketNumber = controllerState.dwPacketNumber;


		// Update buttons state
		const u32 nb_buttons = buttons_.size();
		for(u32 b = 0; b < nb_buttons; ++b)
		{
			buttons_[b].setState( (controllerState.Gamepad.wButtons & buttons_[b].id()) != 0 );
		}
		
		// Update axis state
		axis_[eX360_LeftX].setRawValue(float(controllerState.Gamepad.sThumbLX));
		axis_[eX360_LeftY].setRawValue(float(controllerState.Gamepad.sThumbLY));
		axis_[eX360_RightX].setRawValue(float(controllerState.Gamepad.sThumbRX));
		axis_[eX360_RightY].setRawValue(float(controllerState.Gamepad.sThumbRY));

		// Update triggers state
		triggers_[eX360_LeftTrigger].setRawValue(float(controllerState.Gamepad.bLeftTrigger));
		triggers_[eX360_RightTrigger].setRawValue(float(controllerState.Gamepad.bRightTrigger));
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void gamepad::vibrate(float val)
	{
		vibrate(val, val);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void gamepad::vibrate(float left, float right)
	{
		// Create a Vibration State
		XINPUT_VIBRATION vibration;

		// Clean the vibration
		memset(&vibration, 0, sizeof(XINPUT_VIBRATION));

		// Set the Vibration Values
		vibration.wLeftMotorSpeed  = static_cast<u16>(left  * std::numeric_limits<u16>::max());
		vibration.wRightMotorSpeed = static_cast<u16>(right * std::numeric_limits<u16>::max());

		// Vibrate the controller
		XInputSetState(0, &vibration);
	}
	//----------------------------------------------------------------------------------------------

} /*input*/ } /*system*/ } /*djah*/