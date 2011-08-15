#include "animations.hpp"
#include "common.hpp"

namespace collada { namespace library {

	//----------------------------------------------------------------------------------------------
	void animations::deserialize(const TiXmlElement &element)
	{
		const TiXmlElement *animation_elem = element.FirstChildElement("animation");
		while( animation_elem )
		{
			animation *new_animation = new animation;
			new_animation->deserialize(*animation_elem);
			animations_.push_back(new_animation);

			animation_elem = animation_elem->NextSiblingElement("animation");
		}
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

		const TiXmlElement *source_elem = element.FirstChildElement("source");
		while( source_elem )
		{
			source *new_source = new source;
			new_source->deserialize(*source_elem);
			sources_.push_back(new_source);
			source_elem = source_elem->NextSiblingElement("source");
		}

		const TiXmlElement *sampler_elem = element.FirstChildElement("sampler");
		if( sampler_elem )
		{
			sampler_ = new sampler;
			sampler_->deserialize(*sampler_elem);
		}

		const TiXmlElement *channel_elem = element.FirstChildElement("channel");
		if( channel_elem )
		{
			channel_ = new channel;
			channel_->deserialize(*channel_elem);
		}
	}
	//----------------------------------------------------------------------------------------------
	animation::~animation()
	{
		delete_content(sources_);
		delete sampler_;
		delete channel_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void sampler::deserialize(const TiXmlElement &element)
	{
		if(element.Attribute("id"))
			id_ = element.Attribute("id");

		const TiXmlElement *input_elem = element.FirstChildElement("input");
		while( input_elem )
		{
			input *new_input = new input;
			new_input->deserialize(*input_elem);
			inputs_.push_back(new_input);
			input_elem = input_elem->NextSiblingElement("input");
		}
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