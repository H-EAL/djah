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
	class image;
	class material;
	class effect;
	class geometry;
	class controller;
	class visual_scene;
	//----------------------------------------------------------------------------------------------
	typedef std::vector<animation*>		animation_list_t;
	typedef std::vector<image*>			image_list_t;
	typedef std::vector<material*>		material_list_t;
	typedef std::vector<effect*>		effect_list_t;
	typedef std::vector<geometry*>		geometry_list_t;
	typedef std::vector<controller*>	controller_list_t;
	typedef std::vector<visual_scene*>	visual_scene_list_t;
	//----------------------------------------------------------------------------------------------

} /*library*/ } /*collada*/

#endif /* COLLADA_FWD_DECL_HPP */