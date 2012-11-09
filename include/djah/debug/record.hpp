#ifndef DJAH_DEBUG_RECORD_HPP
#define DJAH_DEBUG_RECORD_HPP

#include <string>
#include <ostream>
#include <sstream>

namespace djah { namespace debug {

	//----------------------------------------------------------------------------------------------
	enum eLogSeverity
	{
		use_last,
		notification,
		warning,
		error,
		critical,

		eLS_Count
	};
	//----------------------------------------------------------------------------------------------
	inline std::ostream& operator <<(std::ostream &out, eLogSeverity sev)
	{
		static const char *sevStr[] =
		{
			"",
			"notification",
			"warning",
			"error",
			"critical",
		};
		return out << sevStr[sev];
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	class basic_record
	{
	public:
		basic_record
		(
			const std::string &_channels = "all",
			eLogSeverity _severity = notification,
			unsigned int _line = 0, const std::string &_file = "",
			const std::string &_stackTrace = "",
			signed int _timestamp = 0
		)
		: channels_(_channels)
		, severity_(_severity == use_last ? currentSeverity : _severity)
		, line_(_line)
		, file_(_file)
		, stackTrace_(_stackTrace)
		, timestamp_(_timestamp)
		, message_("")
		{
			currentSeverity = severity_;
		}


		const std::string&	channels()	const { return channels_;	}
		eLogSeverity		severity()	const { return severity_;	}
		unsigned int 		line()		const { return line_;		}
		const std::string&	file()		const { return file_;		}
		const std::string&	stackTrace()const { return stackTrace_;	}
		unsigned int 		timestamp()	const { return timestamp_;	}
		const std::string&	message()	const { return message_;	}
		std::string&		message()		  { return message_;	}

	private:
		std::string  channels_;
		eLogSeverity severity_;
		unsigned int line_;
		std::string	 file_;
		std::string	 stackTrace_;
		unsigned int timestamp_;
		std::string	 message_;

		static eLogSeverity currentSeverity;
	};

	template<typename T>
	basic_record& operator <<(basic_record &lhs, const T &rhs)
	{
		std::stringstream ss;
		ss << rhs;
		lhs.message() += ss.str();
		return lhs;
	}

} /*debug*/ } /*djah*/

#endif /* DJAH_DEBUG_RECORD_HPP */