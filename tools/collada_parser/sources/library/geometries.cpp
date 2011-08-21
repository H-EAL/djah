#include "library/geometries.hpp"
#include "library/common.hpp"

namespace collada { namespace library {

	//----------------------------------------------------------------------------------------------
	void geometries::deserialize(const TiXmlElement &element)
	{
		new_multi_elements(element, "geometry", geometries_);
	}
	//----------------------------------------------------------------------------------------------
	geometries::~geometries()
	{
		delete_content(geometries_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void geometry::deserialize(const TiXmlElement &element)
	{
		if(element.Attribute("id"))
			id_   = element.Attribute("id");
		if(element.Attribute("name"))
			name_ = element.Attribute("name");

		new_single_element(element, "mesh", mesh_);
	}
	//----------------------------------------------------------------------------------------------
	geometry::~geometry()
	{
		delete mesh_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void mesh::deserialize(const TiXmlElement &element)
	{
		new_multi_elements(element, "source", sources_);
		new_single_element(element, "vertices",	vertices_);
		new_multi_elements(element, "triangles", triangles_);
	}
	//----------------------------------------------------------------------------------------------
	mesh::~mesh()
	{
		delete_content(triangles_);
		delete vertices_;
		delete_content(sources_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void vertices::deserialize(const TiXmlElement &element)
	{
		if(element.Attribute("id"))
			id_ = element.Attribute("id");
		
		new_multi_elements(element, "input", inputs_);
	}
	//----------------------------------------------------------------------------------------------
	vertices::~vertices()
	{
		delete_content(inputs_);
	}
	//-----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void triangles::deserialize(const TiXmlElement &element)
	{
		int tmp = 0;
		element.Attribute("count", &tmp);
		count_ = static_cast<unsigned int>(tmp);
		material_ = element.Attribute("material");

		unsigned int max_offset = 0;
		const TiXmlElement *input_elem = element.FirstChildElement("input");
		while( input_elem )
		{
			input *new_input = new input;
			new_input->deserialize(*input_elem);
			max_offset = std::max(new_input->offset_, max_offset);
			inputs_.push_back(new_input);
			input_elem = input_elem->NextSiblingElement("input");
		}

		const TiXmlElement *p_elem = element.FirstChildElement("p");
		if( p_elem && count_ )
		{
			real_count_ = 3 * count_ * (max_offset+1);
			indices_ = new unsigned short[real_count_];
			std::stringstream ss(p_elem->GetText());
			for(unsigned int i = 0; i < real_count_ && !ss.eof(); ++i)
				ss >> indices_[i];
		}
	}
	//----------------------------------------------------------------------------------------------
	triangles::~triangles()
	{
		delete_content(inputs_);
		delete [] indices_;
	}
	//----------------------------------------------------------------------------------------------
	
} /*library*/ } /*collada*/