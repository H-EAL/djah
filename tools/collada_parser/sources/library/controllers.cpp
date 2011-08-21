#include "library/controllers.hpp"
#include <numeric>
#include "library/common.hpp"

namespace collada { namespace library {

	//----------------------------------------------------------------------------------------------
	void controllers::deserialize(const TiXmlElement &element)
	{
		new_multi_elements(element, "controller", controllers_);
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

		new_single_element(element, "skin", skin_);
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

		new_multi_elements(element, "source", sources_);
		new_single_element(element, "joints", joints_);
		new_single_element(element, "vertex_weights", vertex_weights_);
	}
	//----------------------------------------------------------------------------------------------
	skin::~skin()
	{
		delete vertex_weights_;
		delete joints_;
		delete_content(sources_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void joints::deserialize(const TiXmlElement &element)
	{
		new_multi_elements(element, "input", inputs_);
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
		
		new_multi_elements(element, "input", inputs_);

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
		delete [] v_;
		delete [] vcount_;
		delete_content(inputs_);
	}
	//----------------------------------------------------------------------------------------------

} /*library*/ } /*collada*/