#ifndef COLLADA_LIBRARY_MATERIALS_HPP
#define COLLADA_LIBRARY_MATERIALS_HPP

#include <string>
#include "../fwd_decl.hpp"

namespace collada { namespace library {

	//----------------------------------------------------------------------------------------------
	class instance_effect;
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	class materials
	{
	public:
		void deserialize(const TiXmlElement &element);
		~materials();

		material_list_t materials_;
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class material
	{
	public:
		material() : instance_effect_(0) {}
		void deserialize(const TiXmlElement &element);
		~material();

		std::string		id_;
		std::string		name_;
		instance_effect *instance_effect_;
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class instance_effect
	{
	public:
		void deserialize(const TiXmlElement &element);

		std::string	url_;
	};
	//----------------------------------------------------------------------------------------------

} /*library*/ } /*collada*/

#endif /* COLLADA_LIBRARY_MATERIALS_HPP */