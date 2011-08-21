#include "library/animations.hpp"
#include "library/common.hpp"

namespace collada { namespace library {

	//----------------------------------------------------------------------------------------------
	void animations::deserialize(const TiXmlElement &element)
	{
		new_multi_elements(element, "animation", animations_);
	}
	//----------------------------------------------------------------------------------------------
	animations::~animations()
	{
		delete_content(animations_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void animation::deserialize(const TiXmlElement &element)
	{
		if(element.Attribute("id"))
			id_   = element.Attribute("id");
		
		new_multi_elements(element, "source", sources_);
		new_single_element(element, "sampler", sampler_);
		new_single_element(element, "channel", channel_);
	}
	//----------------------------------------------------------------------------------------------
	animation::~animation()
	{
		delete sampler_;
		delete channel_;
		delete_content(sources_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void sampler::deserialize(const TiXmlElement &element)
	{
		if(element.Attribute("id"))
			id_ = element.Attribute("id");
		
		new_multi_elements(element, "input", inputs_);
	}
	//----------------------------------------------------------------------------------------------
	sampler::~sampler()
	{
		delete_content(inputs_);
	}
	//-----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void channel::deserialize(const TiXmlElement &element)
	{
		if(element.Attribute("source"))
			source_ = element.Attribute("source");
		if(source_[0] == '#') source_ = source_.substr(1);

		if(element.Attribute("target"))
			target_ = element.Attribute("target");
	}
	//----------------------------------------------------------------------------------------------

} /*library*/ } /*collada*/