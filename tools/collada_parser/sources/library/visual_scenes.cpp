#include "library/visual_scenes.hpp"
#include "library/common.hpp"

namespace collada { namespace library {

	//----------------------------------------------------------------------------------------------
	void visual_scenes::deserialize(const TiXmlElement &element)
	{
		const TiXmlElement *visual_scene_elem = element.FirstChildElement("visual_scene");
		while( visual_scene_elem )
		{
			visual_scene *new_visual_scene = new visual_scene;
			new_visual_scene->deserialize(*visual_scene_elem);
			visual_scenes_.push_back(new_visual_scene);

			visual_scene_elem = visual_scene_elem->NextSiblingElement("visual_scene");
		}
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

		const TiXmlElement *node_elem = element.FirstChildElement("node");
		while( node_elem )
		{
			node *new_node = new node;
			new_node->deserialize(*node_elem);
			nodes_.push_back(new_node);
			node_elem = node_elem->NextSiblingElement("node");
		}
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

		const TiXmlElement *inst_controller_element = element.FirstChildElement("instance_controller");
		if(inst_controller_element)
		{
			instance_controller_ = new instance_controller;
			instance_controller_->deserialize(*inst_controller_element);
		}
	}
	//----------------------------------------------------------------------------------------------
	node::~node()
	{
		for(int i = 0; i < transformation::ETT_COUNT; ++i)
			delete_content(transformations_[i]);
		parent_ = 0;
		delete_content(children_);
		delete instance_controller_;
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