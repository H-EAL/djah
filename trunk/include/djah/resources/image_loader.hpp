#ifndef DJAH_RESOURCES_IMAGE_LOADER_HPP
#define DJAH_RESOURCES_IMAGE_LOADER_HPP

#include "loader.hpp"
#include "image.hpp"

namespace djah { namespace resources {

	class image_loader
		: public loader<image>
	{
	public:
		image_loader();
		virtual ~image_loader();
		image* loadFromStream(filesystem::stream &stream, const std::string &filename = "");
	};

} /*resources*/ } /*djah*/

#endif /* DJAH_RESOURCES_IMAGE_LOADER_HPP */