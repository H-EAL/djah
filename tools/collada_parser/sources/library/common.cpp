#include "library/common.hpp"

namespace collada { namespace library {

	//----------------------------------------------------------------------------------------------
	void source::deserialize(const TiXmlElement &element)
	{
		if(element.Attribute("id"))
			id_   = element.Attribute("id");
		if(element.Attribute("name"))
			name_ = element.Attribute("name");

		const TiXmlElement *float_array_elem = element.FirstChildElement("float_array");
		if( float_array_elem )
		{
			float_array_ = new float_array;
			float_array_->deserialize(*float_array_elem);
		}

		const TiXmlElement *name_array_elem = element.FirstChildElement("name_array");
		if( name_array_elem )
		{
			name_array_ = new name_array;
			name_array_->deserialize(*name_array_elem);
		}

		const TiXmlElement *technique_common_elem = element.FirstChildElement("technique_common");
		if( technique_common_elem )
		{
			technique_common_ = new technique_common;
			technique_common_->deserialize(*technique_common_elem);
		}
	}
	//----------------------------------------------------------------------------------------------
	source::~source()
	{
		delete float_array_;
		delete name_array_;
		delete technique_common_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void input::deserialize(const TiXmlElement &element)
	{
		int tmp = 0;
		element.Attribute("offset", &tmp);
		offset_ = static_cast<unsigned int>(tmp);
		if(element.Attribute("semantic"))
			semantic_ = element.Attribute("semantic");
		if(element.Attribute("source"))
			source_ = element.Attribute("source");
		if(source_[0] == '#') source_ = source_.substr(1);
		tmp = 0;
		element.Attribute("set", &tmp);
		set_ = static_cast<unsigned int>(tmp);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void technique_common::deserialize(const TiXmlElement &element)
	{
		const TiXmlElement *accessor_elem = element.FirstChildElement("accessor");
		if( accessor_elem )
		{
			accessor_ = new accessor;
			accessor_->deserialize(*accessor_elem);
		}
	}
	//----------------------------------------------------------------------------------------------
	technique_common::~technique_common()
	{
		delete accessor_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void accessor::deserialize(const TiXmlElement &element)
	{
		int tmp = 0;
		element.Attribute("count", &tmp);
		count_ = static_cast<unsigned int>(tmp);
		tmp = 0;
		element.Attribute("offset", &tmp);
		offset_ = static_cast<unsigned int>(tmp);
		if(element.Attribute("source"))
			source_ = element.Attribute("source");
		if(source_[0] == '#') source_ = source_.substr(1);
		tmp = 0;
		element.Attribute("stride", &tmp);
		stride_ = static_cast<unsigned int>(tmp);

		const TiXmlElement *param_elem = element.FirstChildElement("param");
		while( param_elem )
		{
			param *new_param = new param;
			new_param->deserialize(*param_elem);
			params_.push_back(new_param);
			param_elem = param_elem->NextSiblingElement("param");
		}
	}
	//----------------------------------------------------------------------------------------------
	accessor::~accessor()
	{
		delete_content(params_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void param::deserialize(const TiXmlElement &element)
	{
		if(element.Attribute("name"))
			name_ = element.Attribute("name");
		if(element.Attribute("type"))
			type_ = element.Attribute("type");
	}
	//----------------------------------------------------------------------------------------------

} /*library*/ } /*collada*/