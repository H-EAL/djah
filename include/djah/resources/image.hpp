#ifndef DJAH_RESOURCES_IMAGE_HPP
#define DJAH_RESOURCES_IMAGE_HPP

#include <vector>
#include "resource_base.hpp"

namespace djah { namespace resources {

	class image
		: public resource_base
	{
	private:
		typedef unsigned char byte;

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

} /*resources*/ } /*djah*/

#endif /* DJAH_RESOURCES_IMAGE_HPP */