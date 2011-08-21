#ifndef COLLADA_LIBRARY_EFFECTS_HPP
#define COLLADA_LIBRARY_EFFECTS_HPP

#include "../fwd_decl.hpp"

namespace collada { namespace library {

	//----------------------------------------------------------------------------------------------
	class profile_common;
	class newparam;
	class technique;
	class phong;
	//----------------------------------------------------------------------------------------------
	typedef std::vector<newparam*>	newparam_list_t;
	//----------------------------------------------------------------------------------------------

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
		effect() : profile_common_(0) {}
		void deserialize(const TiXmlElement &element);
		~effect();

		std::string		id_;
		std::string		name_;
		profile_common	*profile_common_;
	};
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	class profile_common
	{
	public:
		profile_common() : technique_(0) {}
		void deserialize(const TiXmlElement &element);
		~profile_common();

		newparam_list_t	newparams_;
		technique		*technique_;
	};
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	class newparam
	{
	public:
		void deserialize(const TiXmlElement &element);
		~newparam();

		std::string sid_;
	};
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	class technique
	{
	public:
		void deserialize(const TiXmlElement &element);
		~technique();

		std::string	sid_;
		phong		*phong_;
	};
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	class phong
	{
	public:
		void deserialize(const TiXmlElement &element);
		~phong();

		typedef float color_t[4];

		color_t emission_;
		color_t ambient_;
		//texture *texture_;
		color_t specular_;
		float	shininess_;
		color_t reflective_;
		float	reflectivity_;
		//transparent *transparent_;
		float	transparency_;
		float	index_of_refraction_;
	};
	//----------------------------------------------------------------------------------------------

} /*library*/ } /*collada*/

#endif /* COLLADA_LIBRARY_EFFECTS_HPP */