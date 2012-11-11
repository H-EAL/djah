#include "djah/debug/console_sink.hpp"

#include <ctime>
#include <iostream>

#include "djah/platform.hpp"

namespace djah { namespace debug {

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


		//------------------------------------------------------------------------------------------
		#ifdef DJAH_COMPILE_WINDOWS
			typedef int color_type;
			static const color_type COLORS[COUNT] =
			{ 15, 0, 7, 12, 10, 9, 4, 2, 1, 8, 13, 14, 11 };
		#else
			typedef char color_type;
			static const color_type COLORS[COUNT] =
			{ '7', '0', '7', '1', '2', '4', '1', '2', '4', '0', '5', '3', '6' };
		#endif
		//------------------------------------------------------------------------------------------


		//------------------------------------------------------------------------------------------
		void _setConsoleColor(color_type textColor, color_type bgColor)
		{
			#ifdef DJAH_COMPILE_WINDOWS
				HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(hdl, static_cast<WORD>(textColor+(bgColor<<4)));
			#else
				std::string col = "3";
				col += textColor;
				col += ";4";
				col += bgColor;
				printf("\033[%sm", col.c_str());
			#endif
		}
		//------------------------------------------------------------------------------------------


		//------------------------------------------------------------------------------------------
		void set_console_color(bool resetColor, int level)
		{
			typedef std::pair<color_type, color_type> color;

			static const color color_levels[eLS_Count] =
			{
				//Text color,          Background color
				color(COLORS[CYAN],    COLORS[BLACK]),
				color(COLORS[GREEN],   COLORS[BLACK]),
				color(COLORS[YELLOW],  COLORS[BLACK]),
				color(COLORS[MAGENTA], COLORS[BLACK]),
				color(COLORS[RED],     COLORS[BLACK])
			};

			if(resetColor)
				_setConsoleColor(COLORS[GRAY], COLORS[BLACK]);
			else
				_setConsoleColor(color_levels[level].first, color_levels[level].second);
		}
		//------------------------------------------------------------------------------------------
	}


	//----------------------------------------------------------------------------------------------
	console_sink::console_sink(const log_filter &_filter)
		: basic_sink(_filter)
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	console_sink::~console_sink()
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void console_sink::consume(const basic_record &rec)
	{
		time_t timestamp = rec.timestamp();
		struct tm t;
		localtime_s(&t, &timestamp);

		std::cout
			<< "[" << (t.tm_hour < 10 ? "0" : "") << t.tm_hour
			<< ":" << (t.tm_min  < 10 ? "0" : "") << t.tm_min
			<< ":" << (t.tm_sec  < 10 ? "0" : "") << t.tm_sec
			<< "] ";

		set_console_color(false, rec.severity());

		std::cout
			<< "[" << rec.channels()  << "] - "
			<< rec.message()
			<< std::endl;

		set_console_color(true, 0);
	}
	//----------------------------------------------------------------------------------------------



	//----------------------------------------------------------------------------------------------
	output_debug_sink::output_debug_sink(const log_filter &_filter)
		: basic_sink(_filter)
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	output_debug_sink::~output_debug_sink()
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void output_debug_sink::consume(const basic_record &rec)
	{
#ifdef DJAH_COMPILE_WINDOWS
		std::stringstream ss;

		time_t timestamp = rec.timestamp();
		struct tm t;
		localtime_s(&t, &timestamp);

		ss
			<< "[" << (t.tm_hour < 10 ? "0" : "") << t.tm_hour
			<< ":" << (t.tm_min  < 10 ? "0" : "") << t.tm_min
			<< ":" << (t.tm_sec  < 10 ? "0" : "") << t.tm_sec
			<< "] "
			<< "[" << rec.channels()  << "] - "
			<< rec.message()
			<< "\n";

		OutputDebugString(ss.str().c_str());
#endif
	}
	//----------------------------------------------------------------------------------------------

} /*debug*/ } /*djah*/