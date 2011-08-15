#include "library/materials.hpp"
#include "library/common.hpp"

namespace collada { namespace library {

	//----------------------------------------------------------------------------------------------
	void materials::deserialize(const TiXmlElement &element)
	{
		const TiXmlElement *material_elem = element.FirstChildElement("material");
		while( material_elem )
		{
			material *new_material = new material;
			new_material->deserialize(*material_elem);
			materials_.push_back(new_material);

			material_elem = material_elem->NextSiblingElement("material");
		}
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

		const TiXmlElement *instance_effect_elem = element.FirstChildElement("instance_effect");
		if( instance_effect_elem )
		{
			instance_effect_ = new instance_effect;
			instance_effect_->deserialize(*instance_effect_elem);
		}
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