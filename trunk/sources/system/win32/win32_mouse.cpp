#include "djah/system/input/mouse.hpp"

#include <cstring>
#include <iostream>
#include "djah/platform.hpp"
#include "djah/system/device.hpp"

namespace djah { namespace system { namespace input {

	//----------------------------------------------------------------------------------------------
	mouse::mouse()
		: left_button_  ("MOUSE_BTN_LEFT"  , VK_LBUTTON)
		, middle_button_("MOUSE_BTN_MIDDLE", VK_MBUTTON)
		, right_button_ ("MOUSE_BTN_RIGHT" , VK_RBUTTON)
	{
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	mouse::~mouse()
	{
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	void mouse::update()
	{
		CURSORINFO ci;
		ci.cbSize = sizeof(CURSORINFO);

		if( GetCursorInfo(&ci) )
		{
			const math::vector2i screenPos(ci.ptScreenPos.x, ci.ptScreenPos.y);
			const math::vector2i &clientPos = system::device::get_current()->clientMousePosition(screenPos);
			delta_	  = clientPos - position_;
			position_ = clientPos;
		}

		SHORT state = GetKeyState( left_button_.id() );
		left_button_.setState( (state & 0x8000) != 0 );

		state = GetKeyState( middle_button_.id() );
		middle_button_.setState( (state & 0x8000) != 0 );

		state = GetKeyState( right_button_.id() );
		right_button_.setState( (state & 0x8000) != 0 );
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	void mouse::setPosition(const math::vector2i &clientPos)
	{
		const math::vector2i &screenPos = system::device::get_current()->screenMousePosition(clientPos);
		if( !!SetCursorPos(screenPos.x, screenPos.y) )
		{
			delta_    = clientPos - position_;
			position_ = clientPos;
		}
	}
	//----------------------------------------------------------------------------------------------

} /*input*/ } /*system*/ } /*djah*/