#include "library/controllers.hpp"
#include <numeric>
#include "library/common.hpp"

namespace collada { namespace library {

	//----------------------------------------------------------------------------------------------
	void controllers::deserialize(const TiXmlElement &element)
	{
		const TiXmlElement *controller_elem = element.FirstChildElement("controller");
		while( controller_elem )
		{
			controller *new_controller = new controller;
			new_controller->deserialize(*controller_elem);
			controllers_.push_back(new_controller);

			controller_elem = controller_elem->NextSiblingElement("geometry");
		}
	}
	//----------------------------------------------------------------------------------------------
	controllers::~controllers()
	{
		delete_content(controllers_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void controller::deserialize(const TiXmlElement &element)
	{
		if(element.Attribute("id"))
			id_   = element.Attribute("id");
		if(element.Attribute("name"))
			name_ = element.Attribute("name");

		const TiXmlElement *skin_elem = element.FirstChildElement("skin");
		if( skin_elem )
		{
			skin_ = new skin;
			skin_->deserialize(*skin_elem);
		}
	}
	//----------------------------------------------------------------------------------------------
	controller::~controller()
	{
		delete skin_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void skin::deserialize(const TiXmlElement &element)
	{
		if(element.Attribute("source"))
			source_ = element.Attribute("source");
		if(source_[0] == '#') source_ = source_.substr(1);

		const TiXmlElement *matrix_elem = element.FirstChildElement("bind_shape_matrix");
		if( matrix_elem )
		{
			std::stringstream ss(matrix_elem->GetText());
			for(unsigned int i = 0; i < 16 && !ss.eof(); ++i)
				ss >> bind_shape_matrix_[i];
		}

		const TiXmlElement *source_elem = element.FirstChildElement("source");
		while( source_elem )
		{
			source *new_source = new source;
			new_source->deserialize(*source_elem);
			sources_.push_back(new_source);
			source_elem = source_elem->NextSiblingElement("source");
		}

		const TiXmlElement *joints_elem = element.FirstChildElement("joints");
		if( joints_elem )
		{
			joints_ = new joints;
			joints_->deserialize(*joints_elem);
		}

		const TiXmlElement *vertex_weights_elem = element.FirstChildElement("vertex_weights");
		if( vertex_weights_elem )
		{
			vertex_weights_ = new vertex_weights;
			vertex_weights_->deserialize(*vertex_weights_elem);
		}
	}
	//----------------------------------------------------------------------------------------------
	skin::~skin()
	{
		delete_content(sources_);
		delete joints_;
		delete vertex_weights_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void joints::deserialize(const TiXmlElement &element)
	{
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
	joints::~joints()
	{
		delete_content(inputs_);
	}
	//-----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void vertex_weights::deserialize(const TiXmlElement &element)
	{
		int tmp = 0;
		element.Attribute("count", &tmp);
		count_ = static_cast<unsigned int>(tmp);

		const TiXmlElement *input_elem = element.FirstChildElement("input");
		while( input_elem )
		{
			input *new_input = new input;
			new_input->deserialize(*input_elem);
			inputs_.push_back(new_input);
			input_elem = input_elem->NextSiblingElement("input");
		}

		if( count_ )
		{
			const TiXmlElement *vcount_elem = element.FirstChildElement("vcount");
			if( vcount_elem )
			{
				vcount_ = new unsigned short[count_];
				std::stringstream ss(vcount_elem->GetText());
				for(unsigned int i = 0; i < count_ && !ss.eof(); ++i)
					ss >> vcount_[i];
			}

			const TiXmlElement *v_elem = element.FirstChildElement("v");
			if( v_elem )
			{
				const unsigned short count = std::accumulate(vcount_, vcount_+count_, 0) * 2;

				v_ = new unsigned short[count];
				std::stringstream ss(v_elem->GetText());
				for(unsigned int i = 0; i < count && !ss.eof(); ++i)
					ss >> v_[i];
			}
		}
	}
	//----------------------------------------------------------------------------------------------
	vertex_weights::~vertex_weights()
	{
		delete_content(inputs_);
		delete [] vcount_;
		delete [] v_;
	}
	//----------------------------------------------------------------------------------------------

} /*library*/ } /*collada*/