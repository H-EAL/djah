#include "library/visual_scenes.hpp"
#include "library/common.hpp"

namespace collada { namespace library {

	//----------------------------------------------------------------------------------------------
	void visual_scenes::deserialize(const TiXmlElement &element)
	{
		new_multi_elements(element, "visual_scene", visual_scenes_);
	}
	//----------------------------------------------------------------------------------------------
	visual_scenes::~visual_scenes()
	{
		delete_content(visual_scenes_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void visual_scene::deserialize(const TiXmlElement &element)
	{
		if(element.Attribute("id"))
			id_   = element.Attribute("id");
		if(element.Attribute("name"))
			name_ = element.Attribute("name");
		
		new_multi_elements(element, "node", nodes_);
	}
	//----------------------------------------------------------------------------------------------
	visual_scene::~visual_scene()
	{
		delete_content(nodes_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void node::deserialize(const TiXmlElement &element)
	{
		if(element.Attribute("id"))
			id_   = element.Attribute("id");
		if(element.Attribute("name"))
			name_ = element.Attribute("name");
		if(element.Attribute("sid"))
			sid_ = element.Attribute("sid");
		if(element.Attribute("type"))
			type_ = element.Attribute("type");

		static const char *transformations_str[] = { "translate", "rotate", "scale" };

		// Transformations
		for(int i = 0; i < transformation::ETT_COUNT; ++i)
		{
			const TiXmlElement *transformation_element = element.FirstChildElement(transformations_str[i]);
			while(transformation_element)
			{
				transformation *new_transformation = new transformation(static_cast<transformation::E_TRANSFORMATION_TYPE>(i));
				new_transformation->deserialize(*transformation_element);
				transformations_[i].push_back(new_transformation);
				transformation_element = transformation_element->NextSiblingElement(transformations_str[i]);
			}

		}

		const TiXmlElement *matrix_elem = element.FirstChildElement("matrix");
		if( matrix_elem )
		{
			std::stringstream ss(matrix_elem->GetText());
			for(unsigned int i = 0; i < 16 && !ss.eof(); ++i)
				ss >> matrix_[i];
		}
		
		const TiXmlElement *node_element = element.FirstChildElement("node");
		while(node_element)
		{
			node *new_node = new node(this);
			new_node->deserialize(*node_element);
			children_.push_back(new_node);
			node_element = node_element->NextSiblingElement("node");
		}
		
		new_single_element(element, "instance_controller", instance_controller_);
	}
	//----------------------------------------------------------------------------------------------
	node::~node()
	{
		delete instance_controller_;
		delete_content(children_);
		parent_ = 0;
		for(int i = 0; i < transformation::ETT_COUNT; ++i)
			delete_content(transformations_[i]);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void transformation::deserialize(const TiXmlElement &element)
	{
		if(element.Attribute("sid"))
			sid_ = element.Attribute("sid");

		std::stringstream ss(element.GetText());
		ss >> values_[0] >> values_[1] >> values_[2];
		if(type_ == ETT_ROTATE)
			ss >> values_[3];
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void instance_controller::deserialize(const TiXmlElement &element)
	{
		if(element.Attribute("url"))
			url_ = element.Attribute("url");
		if(url_[0] == '#') url_ = url_.substr(1);

		const TiXmlElement *skeleton_elem = element.FirstChildElement("skeleton");
		if(skeleton_elem)
			skeleton_ = skeleton_elem->GetText();

		if(skeleton_[0] == '#') skeleton_ = skeleton_.substr(1);
	}
	//----------------------------------------------------------------------------------------------

} /*library*/ } /*collada*/