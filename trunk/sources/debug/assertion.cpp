#include "djah/debug/assertion.hpp"

#include <vector>
#include <sstream>
#include <algorithm>
#include "djah/platform.hpp"

namespace djah { namespace debug {

	//----------------------------------------------------------------------------------------------
	void debugger_break()
	{
		#ifdef DJAH_COMPILE_WINDOWS
			__debugbreak();
		#else
			asm("int $3");
		#endif // DJAH_COMPILE_WINDOWS
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	std::string stack_trace()
	{
		#ifdef DJAH_COMPILE_WINDOWS
			HANDLE process = GetCurrentProcess();
			SymInitialize(process, NULL, TRUE);

			static const unsigned int MAX_STACK_SIZE = 100;
			void * stack[MAX_STACK_SIZE];
			unsigned short frames = CaptureStackBackTrace(0, MAX_STACK_SIZE, stack, NULL);
			SYMBOL_INFO  *pSymbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
			pSymbol->MaxNameLen   = 255;
			pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);

			std::vector<std::string> callStack;
			callStack.reserve(frames-6);
			for(unsigned short i = 1; i < frames-5; ++i)
			{
				SymFromAddr(process, (DWORD64)(stack[i]), 0, pSymbol);

				std::stringstream ss;
				ss << pSymbol->Name << " - 0x" << std::hex << pSymbol->Address;
				if( callStack.empty() || ss.str() != callStack.back() )
					callStack.push_back(ss.str());
			}

			free( pSymbol );
		#endif // DJAH_COMPILE_WINDOWS

		std::string callStackStr;
		std::for_each(callStack.begin(), callStack.end(), [&callStackStr](const std::string &cl)
		{
			callStackStr += cl;
			callStackStr += "\n";
		});

		return callStackStr;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	eAssertResult assert_function(const char *file, int line, const char *expression, const char *description, ...)
	{
		std::stringstream strMessage;

		va_list argPtr;
		static const unsigned int bufferSize = 1<<14;
		static char formattedMessage[bufferSize];
		va_start(argPtr, description);
		vsprintf_s(formattedMessage, bufferSize, description, argPtr);
		va_end(argPtr);


		strMessage
			<< "*************************************************************************\n"
			<< "Assertion failed:\n" 
			<< "=================\n"
			<< "[" << expression << "]\n"
			<< "-------------------------------------------------------------------------\n"
			<< formattedMessage << "\n"
			<< "-------------------------------------------------------------------------\n"
			<< file << ", line " << line << "\n\n"
			<< "*************************************************************************\n"
			<< "Stack trace:\n"
			<< "-------------------------------------------------------------------------\n"
			<< stack_trace() << "\n";

		eAssertResult assertResult = eAR_Ignore;
		#ifdef DJAH_COMPILE_WINDOWS
			int result = MessageBox(0, strMessage.str().c_str(), "Assertion Failed", MB_ABORTRETRYIGNORE);
			switch( result )
			{
			case IDABORT:
				exit(1);
				result = eAR_Abort;
				break;

			case IDRETRY:
				assertResult = eAR_Retry;
				break;

			case IDIGNORE:
				assertResult = (GetAsyncKeyState(VK_SHIFT) & 0x8000) ? eAR_AlwaysIgnore : eAR_Ignore;
				break;
			}
		#endif // DJAH_COMPILE_WINDOWS

		return assertResult;
	}
	//----------------------------------------------------------------------------------------------

} /*debug*/ } /*djah*/