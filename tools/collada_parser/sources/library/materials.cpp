#include "library/materials.hpp"
#include "library/common.hpp"

namespace collada { namespace library {

	//----------------------------------------------------------------------------------------------
	void materials::deserialize(const TiXmlElement &element)
	{
		new_multi_elements(element, "material", materials_);
	}
	//----------------------------------------------------------------------------------------------
	materials::~materials()
	{
		delete_content(materials_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void material::deserialize(const TiXmlElement &element)
	{
		if(element.Attribute("id"))
			id_   = element.Attribute("id");
		if(element.Attribute("name"))
			name_ = element.Attribute("name");
		
		new_single_element(element, "instance_effect", instance_effect_);
	}
	//----------------------------------------------------------------------------------------------
	material::~material()
	{
		delete instance_effect_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void instance_effect::deserialize(const TiXmlElement &element)
	{
		if(element.Attribute("url"))
			url_ = element.Attribute("url");
		if(url_[0] == '#') url_ = url_.substr(1);
	}
	//----------------------------------------------------------------------------------------------

} /*library*/ } /*collada*/