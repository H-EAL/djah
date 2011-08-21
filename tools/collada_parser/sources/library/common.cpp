#include "library/common.hpp"

namespace collada { namespace library {

	//----------------------------------------------------------------------------------------------
	void source::deserialize(const TiXmlElement &element)
	{
		if(element.Attribute("id"))
			id_   = element.Attribute("id");
		if(element.Attribute("name"))
			name_ = element.Attribute("name");

		new_single_element(element, "float_array", float_array_);
		new_single_element(element, "name_array", name_array_);
		new_single_element(element, "technique_common", technique_common_);
	}
	//----------------------------------------------------------------------------------------------
	source::~source()
	{
		delete technique_common_;
		delete name_array_;
		delete float_array_;
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
		new_single_element(element, "accessor", accessor_);
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

		new_multi_elements(element, "param", params_);
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