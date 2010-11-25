#include "log/console_logger.hpp"
#include <iostream>
#include "platform.hpp"

namespace djah { namespace log {

	namespace priv
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
		typedef std::pair<priv::color_type,priv::color_type> color;

		static const color color_levels[EWL_COUNT] =
		{
			//           Text color,              Background color,      Warning level
			color(priv::COLORS[priv::CYAN],    priv::COLORS[priv::BLACK]),	//NOTIFICATION
			color(priv::COLORS[priv::GREEN],   priv::COLORS[priv::BLACK]),	//LOW
			color(priv::COLORS[priv::YELLOW],  priv::COLORS[priv::BLACK]),	//MEDIUM
			color(priv::COLORS[priv::MAGENTA], priv::COLORS[priv::BLACK]),	//HIGH
			color(priv::COLORS[priv::RED],     priv::COLORS[priv::BLACK])	//CRITICAL
		};

		if(resetColor)
			priv::_setConsoleColor(priv::COLORS[priv::GRAY], priv::COLORS[priv::BLACK]);
		else
			priv::_setConsoleColor(color_levels[level_].first, color_levels[level_].second);
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