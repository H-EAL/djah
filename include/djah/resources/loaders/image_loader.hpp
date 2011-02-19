#ifndef DJAH_RESOURCES_LOADERS_IMAGE_LOADER_HPP
#define DJAH_RESOURCES_LOADERS_IMAGE_LOADER_HPP

#include "../loader.hpp"
#include "../image.hpp"

namespace djah { namespace resources { namespace loaders {

	class image_loader
		: public loader<image>
	{
	public:
		image_loader();
		~image_loader();
		image* loadFromStream(filesystem::stream &stream, const std::string &filename = "");
	};

} /*loaders*/ } /*resources*/ } /*djah*/

#endif /* DJAH_RESOURCES_LOADERS_IMAGE_LOADER_HPP */