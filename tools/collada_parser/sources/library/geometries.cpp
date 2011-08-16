#include "library/geometries.hpp"
#include "library/common.hpp"

namespace collada { namespace library {

	//----------------------------------------------------------------------------------------------
	void geometries::deserialize(const TiXmlElement &element)
	{
		const TiXmlElement *geometry_elem = element.FirstChildElement("geometry");
		while( geometry_elem )
		{
			geometry *new_geometry = new geometry;
			new_geometry->deserialize(*geometry_elem);
			geometries_.push_back(new_geometry);

			geometry_elem = geometry_elem->NextSiblingElement("geometry");
		}
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

		const TiXmlElement *mesh_elem = element.FirstChildElement("mesh");
		if( mesh_elem )
		{
			mesh_ = new mesh;
			mesh_->deserialize(*mesh_elem);
		}
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
		const TiXmlElement *source_elem = element.FirstChildElement("source");
		while( source_elem )
		{
			source *new_source = new source;
			new_source->deserialize(*source_elem);
			sources_.push_back(new_source);
			source_elem = source_elem->NextSiblingElement("source");
		}

		const TiXmlElement *vertices_elem = element.FirstChildElement("vertices");
		if( vertices_elem )
		{
			vertices_ = new vertices;
			vertices_->deserialize(*vertices_elem);
		}

		const TiXmlElement *triangles_elem = element.FirstChildElement("triangles");
		while( triangles_elem )
		{
			triangles *new_triangles = new triangles;
			new_triangles->deserialize(*triangles_elem);
			triangles_.push_back(new_triangles);
			triangles_elem = triangles_elem->NextSiblingElement("triangles");
		}
	}
	//----------------------------------------------------------------------------------------------
	mesh::~mesh()
	{
		delete_content(sources_);
		delete vertices_;
		delete_content(triangles_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void vertices::deserialize(const TiXmlElement &element)
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