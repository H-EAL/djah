#include "system/input/mouse.hpp"
#include <cstring>
#include "platform.hpp"
#include <iostream>

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
			math::vector2i ptScreenPos(ci.ptScreenPos.x, ci.ptScreenPos.y);
			delta_		= ptScreenPos - position_;
			position_	= ptScreenPos;
		}

		SHORT state = GetKeyState( left_button_.id() );
		left_button_.setState( (state & 0x8000) != 0 );

		state = GetKeyState( middle_button_.id() );
		middle_button_.setState( (state & 0x8000) != 0 );

		state = GetKeyState( right_button_.id() );
		right_button_.setState( (state & 0x8000) != 0 );
	}
	//----------------------------------------------------------------------------------------------

} /*input*/ } /*system*/ } /*djah*/