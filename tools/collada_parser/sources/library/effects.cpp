#include "effects.hpp"
#include "common.hpp"

namespace collada { namespace library {

	//----------------------------------------------------------------------------------------------
	void effects::deserialize(const TiXmlElement &element)
	{
		const TiXmlElement *effect_elem = element.FirstChildElement("effect");
		while( effect_elem )
		{
			effect_elem = element.NextSiblingElement("effect");
		}
	}
	//----------------------------------------------------------------------------------------------
	effects::~effects()
	{
		delete_content(effects_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------

} /*library*/ } /*collada*/