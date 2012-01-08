#ifndef COLLADA_FWD_DECL_HPP
#define COLLADA_FWD_DECL_HPP

#include <vector>

class TiXmlElement;

namespace collada { 

	class proxy;
	class database;

namespace library {

	//---------------------------------------------------------------------------------------------
	// Common classes
	//---------------------------------------------------------------------------------------------
	class param;
	class input;
	class source;
	class node;
	//---------------------------------------------------------------------------------------------
	typedef std::vector<param*>		param_list_t;
	typedef std::vector<input*>		input_list_t;
	typedef std::vector<source*>	source_list_t;
	typedef std::vector<node*>		node_list_t;
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	// Libraries
	//----------------------------------------------------------------------------------------------
	class animations;
	class images;
	class materials;
	class effects;
	class geometries;
	class controllers;
	class visual_scenes;
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	// Libraries components
	//----------------------------------------------------------------------------------------------
	class animation;
	class sampler;
	class channel;
	class image;
	class material;
	class effect;
	class geometry;
	class mesh;
	class triangles;
	class controller;
	class skin;
	class visual_scene;
	//----------------------------------------------------------------------------------------------
	typedef std::vector<animation*>		animation_list_t;
	typedef std::vector<sampler*>		sampler_list_t;
	typedef std::vector<channel*>		channel_list_t;
	typedef std::vector<image*>			image_list_t;
	typedef std::vector<material*>		material_list_t;
	typedef std::vector<effect*>		effect_list_t;
	typedef std::vector<geometry*>		geometry_list_t;
	typedef std::vector<controller*>	controller_list_t;
	typedef std::vector<visual_scene*>	visual_scene_list_t;
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	// Helpers
	//----------------------------------------------------------------------------------------------
	template<typename T>
	struct type_traits
	{
	private:
		
		template<typename T> struct base_type			{ typedef T BaseType; };
		template<typename T> struct base_type<T*>		{ typedef T BaseType; };
		template<typename T> struct base_type<T&>		{ typedef T BaseType; };
		template<typename T> struct base_type<const T>	{ typedef T BaseType; };
		template<typename T> struct base_type<const T&> { typedef T BaseType; };
		template<typename T> struct base_type<const T*>	{ typedef T BaseType; };

	public:

		typedef typename base_type<T>::BaseType BaseType;

		typedef typename BaseType&				ReferenceType;
		typedef typename const BaseType&		ConstRefType;
		typedef typename BaseType*				PointerType;
		typedef typename const BaseType*		ConstPtrType;
	};
	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline void new_single_element(const TiXmlElement &element, const std::string &element_name, T *&element_ptr)
	{
		const TiXmlElement *elem = element.FirstChildElement(element_name.c_str());
		if( elem )
		{
			element_ptr = new T;
			element_ptr->deserialize(*elem);
		}
	}
	//----------------------------------------------------------------------------------------------
	template<typename C>
	inline void new_multi_elements(const TiXmlElement &element, const std::string &element_name, C &element_container)
	{
		typedef typename type_traits<C::value_type>::BaseType T;
		const TiXmlElement *elem = element.FirstChildElement(element_name.c_str());
		while( elem )
		{
			T *new_T = new T;
			new_T->deserialize(*elem);
			element_container.push_back(new_T);

			elem = elem->NextSiblingElement(element_name.c_str());
		}
	}
	//----------------------------------------------------------------------------------------------

} /*library*/ } /*collada*/

#endif /* COLLADA_FWD_DECL_HPP */