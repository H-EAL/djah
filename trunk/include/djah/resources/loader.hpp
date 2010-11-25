#ifndef DJAH_RESOURCES_LOADER_HPP
#define DJAH_RESOURCES_LOADER_HPP

#include <iosfwd>
#include <string>

namespace djah { namespace fs {
	class stream;
} /*fs*/ } /*djah*/

namespace djah { namespace resources {


	template<typename T>
	class loader
	{
	public:

		virtual ~loader() {}

		virtual T* loadFromStream(fs::stream &stream, const std::string &filename = "")
		{
			return 0;
		}

		virtual void saveToStream(const T &obj, fs::stream &stream, const std::string &filename = "")
		{

		}
	};

} /*resources*/ } /*djah*/

#endif /* DJAH_RESOURCES_LOADER_HPP */