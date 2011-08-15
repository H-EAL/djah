#ifndef COLLADA_LIBRARY_ANIMATIONS_HPP
#define COLLADA_LIBRARY_ANIMATIONS_HPP

#include "../fwd_decl.hpp"

namespace collada { namespace library {

	//----------------------------------------------------------------------------------------------
	class sampler;
	class channel;
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	class animations
	{
	public:
		void deserialize(const TiXmlElement &element);
		~animations();

		animation_list_t animations_;
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class animation
	{
	public:
		animation() : sampler_(0), channel_(0) {}
		void deserialize(const TiXmlElement &element);
		~animation();

		std::string		id_;
		source_list_t	sources_;
		sampler			*sampler_;
		channel			*channel_;
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class sampler
	{
	public:
		void deserialize(const TiXmlElement &element);
		~sampler();

		std::string  id_;
		input_list_t inputs_;
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class channel
	{
	public:
		void deserialize(const TiXmlElement &element);

		std::string source_;
		std::string target_;
	};
	//----------------------------------------------------------------------------------------------

} /*library*/ } /*collada*/

#endif /* COLLADA_LIBRARY_ANIMATIONS_HPP */