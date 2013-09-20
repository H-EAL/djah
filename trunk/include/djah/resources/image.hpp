#ifndef DJAH_RESOURCES_IMAGE_HPP
#define DJAH_RESOURCES_IMAGE_HPP

#include <vector>
#include <memory>
#include "djah/resources/asset.hpp"
#include "djah/types.hpp"

namespace djah { namespace resources {

	class image
		: public asset
	{
	public:
		image(unsigned int _width, u32 _height, u32 _channels, const byte *pPixels);
		virtual ~image() {}

		u32			width()    const { return width_;  }
		u32			height()   const { return height_; }
		u32			channels() const { return channels_; }
		const byte*	pixels()   const { return &pPixels_[0]; }

		virtual u32 size() const { return pPixels_.size(); }

	private:
		unsigned int		width_;
		unsigned int		height_;
		unsigned int		channels_;
		std::vector<byte>	pPixels_;
	};

	typedef std::shared_ptr<image> image_sptr;

} /*resources*/ } /*djah*/

#endif /* DJAH_RESOURCES_IMAGE_HPP */