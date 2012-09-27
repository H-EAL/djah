#ifndef DJAH_DATA_OBJECT_ATTRIBUTE_HPP
#define DJAH_DATA_OBJECT_ATTRIBUTE_HPP

#include <string>

namespace djah { namespace dataobject {

	// An attribute is a name and a value
	template<typename T>
	struct attribute
	{
		attribute(const std::string n = "unknown", const T &v = T())
			: name(n)
			, value(v)
		{}

		std::string name;
		T			value;
	};

} /*dataobject*/ } /*djah*/

#endif /* DJAH_DATA_OBJECT_ATTRIBUTE_HPP */