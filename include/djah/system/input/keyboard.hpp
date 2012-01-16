#ifndef DJAH_SYSTEM_INPUT_KEYBOARD_HPP
#define DJAH_SYSTEM_INPUT_KEYBOARD_HPP

#include <vector>

namespace djah { namespace system { namespace input {

	enum E_KEY_CODE
	{
		EKC_BACKSPACE	= 0x08,
		EKC_TAB			= 0x09,
		EKC_RETURN		= 0x0D,
		EKC_SPACE		= 0x20,

		EKC_0			= 0x30,
		EKC_1, EKC_2, EKC_3, EKC_4, EKC_5, EKC_6, EKC_7, EKC_8, EKC_9,

		EKC_A			= 0x41,
		EKC_B, EKC_C, EKC_D, EKC_E, EKC_F, EKC_G, EKC_H, EKC_I, EKC_J,
		EKC_K, EKC_L, EKC_M, EKC_N, EKC_O, EKC_P, EKC_Q, EKC_R, EKC_S,
		EKC_T, EKC_U, EKC_V, EKC_W, EKC_X, EKC_Y, EKC_Z,
	};

	class keyboard
	{
	public:
		keyboard();
		~keyboard();

		void update();

		bool isKeyDown(E_KEY_CODE key_code) { return (keys_state_[key_code] & 0x8000) != 0; }

	private:
		std::vector<unsigned char> keys_state_;
	};

} /*input*/ } /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_INPUT_KEYBOARD_HPP */