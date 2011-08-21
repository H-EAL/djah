#include "library/effects.hpp"
#include "library/common.hpp"

namespace collada { namespace library {

	//----------------------------------------------------------------------------------------------
	void effects::deserialize(const TiXmlElement &element)
	{
		new_multi_elements(element, "effect", effects_);
	}
	//----------------------------------------------------------------------------------------------
	effects::~effects()
	{
		delete_content(effects_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void effect::deserialize(const TiXmlElement &element)
	{
		if(element.Attribute("id"))
			id_   = element.Attribute("id");
		if(element.Attribute("name"))
			name_ = element.Attribute("name");

		new_single_element(element, "profile_COMMON", profile_common_);
	}
	//----------------------------------------------------------------------------------------------
	effect::~effect()
	{
		delete profile_common_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void profile_common::deserialize(const TiXmlElement &element)
	{
		new_multi_elements(element, "newparam", newparams_);
		new_single_element(element, "technique", technique_);
	}
	//----------------------------------------------------------------------------------------------
	profile_common::~profile_common()
	{
		delete technique_;
		delete_content(newparams_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void newparam::deserialize(const TiXmlElement &element)
	{
		if(element.Attribute("sid"))
			sid_ = element.Attribute("sid");
	}
	//----------------------------------------------------------------------------------------------
	newparam::~newparam()
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void technique::deserialize(const TiXmlElement &element)
	{
		if(element.Attribute("sid"))
			sid_ = element.Attribute("sid");
		
		new_single_element(element, "phong", phong_);
	}
	//----------------------------------------------------------------------------------------------
	technique::~technique()
	{
		delete phong_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void phong::deserialize(const TiXmlElement &element)
	{
	}
	//----------------------------------------------------------------------------------------------
	phong::~phong()
	{
	}
	//----------------------------------------------------------------------------------------------

} /*library*/ } /*collada*/