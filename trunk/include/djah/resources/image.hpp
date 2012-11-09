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
		image(unsigned int _width, u32 _height, u32 _channels, const byte *pPixels);
		~image() {}

		u32			width()    const { return width_;  }
		u32			height()   const { return height_; }
		u32			channels() const { return channels_; }
		const byte*	pixels()   const { return &pPixels_[0]; }

	private:
		unsigned int		width_;
		unsigned int		height_;
		unsigned int		channels_;
		std::vector<byte>	pPixels_;
	};

	typedef std::shared_ptr<image> image_ptr;

} /*resources*/ } /*djah*/

#endif /* DJAH_RESOURCES_IMAGE_HPP */