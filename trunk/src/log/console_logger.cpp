#include "log/console_logger.hpp"
#include <iostream>
#include "platform.hpp"

namespace djah { namespace log {

	namespace
	{
		//----------------------------------------------------------------------
		enum
		{
			WHITE,
			BLACK,
			GRAY,
			RED,
			GREEN,
			BLUE,
			DARK_RED,
			DARK_GREEN,
			DARK_BLUE,
			DARK_GRAY,
			MAGENTA,
			YELLOW,
			CYAN,
			COUNT
		};
		//----------------------------------------------------------------------


		//----------------------------------------------------------------------
		#ifdef DJAH_COMPILE_WINDOWS
			typedef int color_type;
			static const color_type COLORS[COUNT] =
			{ 15, 0, 7, 12, 10, 9, 4, 2, 1, 8, 13, 14, 11 };
		#else
			typedef char color_type;
			static const color_type COLORS[COUNT] =
			{ '7', '0', '7', '1', '2', '4', '1', '2', '4', '0', '5', '3', '6' };
		#endif
		//----------------------------------------------------------------------


		//----------------------------------------------------------------------
		void _setConsoleColor(color_type textColor, color_type bgColor)
		{
		#ifdef DJAH_COMPILE_WINDOWS
			HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hdl, static_cast<WORD>(textColor+(bgColor<<4)));
		#else
			std::string col = "3" + textColor + ";" + "4" + bgColor;
			printf("\033[%sm", col.c_str());
		#endif
		}
		//----------------------------------------------------------------------
	}




	//--------------------------------------------------------------------------
	void console_logger::setConsoleColor(bool resetColor) const
	{
		typedef std::pair<color_type, color_type> color;

		static const color color_levels[EWL_COUNT] =
		{
			//           Text color,              Background color,      Warning level
			color(COLORS[CYAN],    COLORS[BLACK]),	//NOTIFICATION
			color(COLORS[GREEN],   COLORS[BLACK]),	//LOW
			color(COLORS[YELLOW],  COLORS[BLACK]),	//MEDIUM
			color(COLORS[MAGENTA], COLORS[BLACK]),	//HIGH
			color(COLORS[RED],     COLORS[BLACK])	//CRITICAL
		};

		if(resetColor)
			_setConsoleColor(COLORS[GRAY], COLORS[BLACK]);
		else
			_setConsoleColor(color_levels[level_].first, color_levels[level_].second);
	}
	//--------------------------------------------------------------------------



	//--------------------------------------------------------------------------
	void console_logger::beginLog()
	{
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	void console_logger::write(const std::string &msg)
	{
		setConsoleColor();
		std::cout << msg;
		setConsoleColor(true);
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	std::string console_logger::endLog() const
	{
		return "\n";
	}
	//--------------------------------------------------------------------------

} /*log*/ } /*djah*/