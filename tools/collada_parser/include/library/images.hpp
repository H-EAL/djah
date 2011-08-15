#ifndef COLLADA_LIBRARY_IMAGES_HPP
#define COLLADA_LIBRARY_IMAGES_HPP

#include <string>
#include "../fwd_decl.hpp"

namespace collada { namespace library {

	//----------------------------------------------------------------------------------------------
	class images
	{
	public:
		void deserialize(const TiXmlElement &element);
		~images();

		image_list_t images_;
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class image
	{
	public:
		void deserialize(const TiXmlElement &element);

		std::string id_;
		std::string name_;
		int			depth_;
		std::string init_from_fullpath_;
		std::string init_from_filename_;
	};
	//----------------------------------------------------------------------------------------------

} /*library*/ } /*collada*/

#endif /* COLLADA_LIBRARY_IMAGES_HPP */