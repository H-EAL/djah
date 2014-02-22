#ifndef DJAH_STRING_UTILS_HPP
#define DJAH_STRING_UTILS_HPP

#include <vector>
#include <cctype>
#include <string>
#include <sstream>
#include <algorithm>

namespace djah { namespace string_utils {

	//----------------------------------------------------------------------------------------------
	typedef std::vector<std::string> string_list_t;
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T1>
	inline void extract_params_from_string(const std::string &params, T1 &t1)
	{
		std::stringstream ss(params);
		ss >> t1;
	}
	//----------------------------------------------------------------------------------------------
	template<typename T1, typename T2>
	inline void extract_params_from_string(const std::string &params, T1 &t1, T2 &t2)
	{
		std::stringstream ss(params);
		ss >> t1 >> t2;
	}
	//----------------------------------------------------------------------------------------------
	template<typename T1, typename T2, typename T3>
	inline void extract_params_from_string(const std::string &params, T1 &t1, T2 &t2, T3 &t3)
	{
		std::stringstream ss(params);
		ss >> t1 >> t2 >> t3;
	}
	//----------------------------------------------------------------------------------------------
	template<typename T1, typename T2, typename T3, typename T4>
	inline void extract_params_from_string(const std::string &params, T1 &t1, T2 &t2, T3 &t3, T4 &t4)
	{
		std::stringstream ss(params);
		ss >> t1 >> t2 >> t3 >> t4;
	}
	//----------------------------------------------------------------------------------------------
	template<typename T1, typename T2, typename T3, typename T4, typename T5>
	inline void extract_params_from_string(const std::string &params, T1 &t1, T2 &t2, T3 &t3, T4 &t4, T5 &t5)
	{
		std::stringstream ss(params);
		ss >> t1 >> t2 >> t3 >> t4 >> t5;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename P>
	inline std::string param_to_string(P p)
	{
		std::stringstream ss;
		ss << p;
		return ss.str();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	inline std::string get_file_extension(const std::string &file_name)
	{
		return file_name.substr(file_name.find_last_of('.')+1);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	inline std::string to_lower_case(std::string str)
	{
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		return str;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	inline void split_string(const std::string &str, string_list_t &tokens, const std::string &delimiters = " ")
	{
		// Skip delimiters at beginning.
		std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
		// Find first "non-delimiter".
		std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

		while(std::string::npos != pos || std::string::npos != lastPos)
		{
			// Found a token, add it to the vector.
			tokens.push_back(str.substr(lastPos, pos - lastPos));
			// Skip delimiters.  Note the "not_of"
			lastPos = str.find_first_not_of(delimiters, pos);
			// Find next "non-delimiter"
			pos = str.find_first_of(delimiters, lastPos);
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	inline std::string trimmed(const std::string &str)
	{
		std::string::size_type firstChar = str.find_first_not_of(" \t\r\n");
		std::string::size_type lastChar  = str.find_last_not_of(" \t\r\n");
		if( firstChar == std::string::npos )
			firstChar = 0;

		return str.substr(firstChar, lastChar == std::string::npos ? std::string::npos : (lastChar - firstChar + 1) );
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	inline bool begins_with(const std::string &str, const std::string &beginingStr)
	{
		const std::string::size_type beginingSize = beginingStr.size();
		bool result = beginingSize <= str.size();
		if( result )
		{
			for(std::string::size_type i = 0; i < beginingSize && result; ++i)
			{
				result = (str[i] == beginingStr[i]);
			}
		}
		return result;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	inline bool ends_with(const std::string &str, const std::string &endStr)
	{
		const std::string::size_type endSize = endStr.size();
		const std::string::size_type strSize = str.size();
		bool result = endSize <= strSize;
		if( result )
		{
			for(std::string::size_type i = 0; i < endSize && result; ++i)
			{
				result = (str[strSize-i-1] == endStr[endSize-i-1]);
			}
		}
		return result;
	}
	//----------------------------------------------------------------------------------------------

} /*string_utils*/ } /*djah*/

#endif /* DJAH_STRING_UTILS_HPP */