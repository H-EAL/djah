#ifndef DJAH_RESOURCES_IMAGE_LOADER_HPP
#define DJAH_RESOURCES_IMAGE_LOADER_HPP

#include "djah/filesystem/stream.hpp"
#include "djah/resources/image.hpp"

namespace djah { namespace resources {

	class image_loader
	{
	public:
		image_loader();
		~image_loader();
		static image_sptr loadFromStream(filesystem::stream &stream, const std::string &url = "");
	};

} /*resources*/ } /*djah*/

#endif /* DJAH_RESOURCES_IMAGE_LOADER_HPP */