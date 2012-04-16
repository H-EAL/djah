#ifndef DJAH_RESOURCES_IMAGE_HPP
#define DJAH_RESOURCES_IMAGE_HPP

#include <vector>
#include <memory>
#include "resource_base.hpp"
#include "../types.hpp"

namespace djah { namespace resources {

	class image
		: public resource_base
	{
	public:
		image(unsigned int width, unsigned int height, const byte *pixels);
		~image() {}

		unsigned int			width()  const { return width_;  }
		unsigned int			height() const { return height_; }
		const unsigned char*	pixels() const { return &pixels_[0]; }

	private:
		unsigned int		width_;
		unsigned int		height_;
		std::vector<byte>	pixels_;
	};

	typedef std::shared_ptr<image> image_ptr;

} /*resources*/ } /*djah*/

#endif /* DJAH_RESOURCES_IMAGE_HPP */