#ifndef DJAH_SYSTEM_INPUT_KEYBOARD_HPP
#define DJAH_SYSTEM_INPUT_KEYBOARD_HPP

#include <vector>

namespace djah { namespace system { namespace input {

	enum eKeyCode
	{
		eKC_BACKSPACE	= 0x08,
		eKC_TAB			= 0x09,
		eKC_RETURN		= 0x0D,
		eKC_SHIFT		= 0x10,
		eKC_ESCAPE		= 0x1B,
		eKC_SPACE		= 0x20,

		eKC_0			= 0x30,
		eKC_1, eKC_2, eKC_3, eKC_4, eKC_5, eKC_6, eKC_7, eKC_8, eKC_9,

		eKC_A			= 0x41,
		eKC_B, eKC_C, eKC_D, eKC_E, eKC_F, eKC_G, eKC_H, eKC_I, eKC_J,
		eKC_K, eKC_L, eKC_M, eKC_N, eKC_O, eKC_P, eKC_Q, eKC_R, eKC_S,
		eKC_T, eKC_U, eKC_V, eKC_W, eKC_X, eKC_Y, eKC_Z,
	};

	class keyboard
	{
	public:
		keyboard();
		~keyboard();

		void update();

		bool isKeyDown (eKeyCode keyCode) const;
		bool wasKeyDown(eKeyCode keyCode) const;

		bool pressed (eKeyCode keyCode) const;
		bool released(eKeyCode keyCode) const;

	private:
		unsigned int				currentState_;
		std::vector<unsigned char>	keysState_[2];
	};

} /*input*/ } /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_INPUT_KEYBOARD_HPP */