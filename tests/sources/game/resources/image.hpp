#ifndef GAME_RESOURCES_IMAGE_HPP
#define GAME_RESOURCES_IMAGE_HPP

#include <vector>
#include <memory>
#include "djah/resources/asset.hpp"
#include "djah/types.hpp"

namespace game { namespace resources {

	class image
		: public djah::resources::asset
	{
	public:
        image(unsigned int _width, djah::u32 _height, djah::u32 _channels, const djah::byte *pPixels);
		virtual ~image() {}

        djah::u32			width()    const { return width_; }
        djah::u32			height()   const { return height_; }
        djah::u32			channels() const { return channels_; }
        const djah::byte*	pixels()   const { return &pPixels_[0]; }

        virtual djah::u32 size() const override { return pPixels_.size(); }

	private:
		unsigned int		    width_;
		unsigned int		    height_;
		unsigned int		    channels_;
        std::vector<djah::byte>	pPixels_;
	};

	typedef std::shared_ptr<image> image_sptr;

} /*resources*/ } /*game*/

#endif /* GAME_RESOURCES_IMAGE_HPP */