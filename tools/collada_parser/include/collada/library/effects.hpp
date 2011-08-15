#ifndef COLLADA_LIBRARY_EFFECTS_HPP
#define COLLADA_LIBRARY_EFFECTS_HPP

#include "../fwd_decl.hpp"

namespace collada { namespace library {

	//----------------------------------------------------------------------------------------------
	class effects
	{
	public:
		void deserialize(const TiXmlElement &element);
		~effects();

		effect_list_t effects_;
	};
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	class effect
	{
	public:
	};
	//----------------------------------------------------------------------------------------------

} /*library*/ } /*collada*/

#endif /* COLLADA_LIBRARY_EFFECTS_HPP */