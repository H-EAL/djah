#include "library/images.hpp"
#include "library/common.hpp"

namespace collada { namespace library {

	//----------------------------------------------------------------------------------------------
	void images::deserialize(const TiXmlElement &element)
	{
		new_multi_elements(element, "image", images_);
	}
	//----------------------------------------------------------------------------------------------
	images::~images()
	{
		delete_content(images_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void image::deserialize(const TiXmlElement &element)
	{
		if(element.Attribute("id"))
			id_   = element.Attribute("id");
		if(element.Attribute("name"))
			name_ = element.Attribute("name");
		element.Attribute("depth", &depth_);

		const TiXmlElement *init_from_elem = element.FirstChildElement("init_from");
		if(init_from_elem)
		{
			init_from_fullpath_ = init_from_elem->GetText();
			size_t pos = init_from_fullpath_.find_last_of("/\\");
			init_from_filename_ = (pos != std::string::npos) 
				? init_from_fullpath_.substr(pos+1) 
				: init_from_fullpath_;
		}
	}
	//----------------------------------------------------------------------------------------------

} /*library*/ } /*collada*/