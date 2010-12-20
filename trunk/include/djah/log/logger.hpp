#ifndef DJAH_LOG_LOGGER_HPP
#define DJAH_LOG_LOGGER_HPP

#include <string>
#include <sstream>

//------------------------------------------------------------------------------
// Inputs todo notes in output console
#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "("__STR1__(__LINE__)") : TODO: "
#ifdef _MSC_VER	
	#define DJAH_LOG_TODO(x) __pragma( message(__LOC__ #x) )
#else
	#define DJAH_LOG_TODO(x)  _Pragma( message(__LOC__ #x) )
#endif
//------------------------------------------------------------------------------
// Macro to ease the logging
#define DJAH_BEGIN_LOG(level)	djah::log::logger::log(djah::log::level)
#define DJAH_END_LOG()			djah::log::logger::endl()
//------------------------------------------------------------------------------


namespace djah { namespace log {

	enum E_WARNING_LEVEL
	{
		EWL_NOTIFICATION,
		EWL_LOW,
		EWL_MEDIUM,
		EWL_HIGH,
		EWL_CRITICAL,
		EWL_COUNT,

		EWL_USELAST
	};

	class logger_impl;
	class logger
	{
	public:

		logger();
		// Abstract class
		virtual ~logger();

		// Change the current logger
		static void setLogger(logger_impl *l);
		static void setLevel(E_WARNING_LEVEL level);

		// C style logging
		static void log(const char *format, ...);
		static std::string endl();

		// C++ style logging
		static logger_impl& log(E_WARNING_LEVEL level = EWL_USELAST);

	private:

		virtual void beginLog() = 0;
		virtual void write(const std::string &msg) = 0;
		virtual std::string endLog() const = 0;

		// Current logger
		static logger_impl *instance_;
	};

	class logger_impl
	{
		friend class logger;

	public:
		template<typename T>
		logger_impl& operator <<(const T &msg);

	protected:
		static E_WARNING_LEVEL level_;

	private:
		virtual void beginLog() = 0;
		virtual void write(const std::string &msg) = 0;
		virtual std::string endLog() const = 0;
	};

	// Useful when we need to disable the logger
	class null_logger
		: public logger_impl
	{
		virtual void beginLog() {};
		virtual void write(const std::string &msg) {};
		virtual std::string endLog() const { return ""; };
	};

} /*log*/ } /*djah*/

#include "logger.inl"

#endif /* DJAH_LOG_LOGGER_HPP */