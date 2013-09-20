#ifndef DJAH_SYSTEM_INPUT_MOUSE_HPP
#define DJAH_SYSTEM_INPUT_MOUSE_HPP

#include "djah/math/vector2.hpp"
#include "button.hpp"

namespace djah { namespace system { namespace input {

	class mouse
	{
	public:
		mouse();
		~mouse();

		void showCursor() { setCursorVisibility(true);  }
		void hideCursor() { setCursorVisibility(false); }
		void setCursorVisibility(bool visible);

		void update();

		const button&			leftButton()	const { return left_button_;	}
		const button&			middleButton()	const { return middle_button_;	}
		const button&			rightButton()	const { return right_button_;	}

		const math::vector2i&	position()		const { return position_;		}
		const math::vector2i&	delta()			const { return delta_;			}

		void					setPosition(const math::vector2i &clientPos);

		bool					hasMoved()		const { return delta_.x != 0 || delta_.y != 0; }

	private:
		button			left_button_;
		button			middle_button_;
		button			right_button_;

		math::vector2i	position_;
		math::vector2i	delta_;
	};

} /*input*/ } /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_INPUT_MOUSE_HPP */