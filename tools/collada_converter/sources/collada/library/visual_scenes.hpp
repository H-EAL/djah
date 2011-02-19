#ifndef COLLADA_LIBRARY_VISUAL_SCENES_HPP
#define COLLADA_LIBRARY_VISUAL_SCENES_HPP

#include "../fwd_decl.hpp"

namespace collada { namespace library {

	//----------------------------------------------------------------------------------------------
	class instance_controller;
	class transformation;
	//----------------------------------------------------------------------------------------------
	typedef std::vector<transformation*> transformation_list_t;
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	class visual_scenes
	{
	public:
		void deserialize(const TiXmlElement &element);
		~visual_scenes();

		visual_scene_list_t visual_scenes_;
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class visual_scene
	{
	public:
		void deserialize(const TiXmlElement &element);
		~visual_scene();

		std::string id_;
		std::string name_;
		node_list_t nodes_;
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class node
	{
	public:
		node(node *parent = 0) : instance_controller_(0), parent_(parent) {}
		void deserialize(const TiXmlElement &element);
		~node();

		std::string				id_;
		std::string				name_;
		std::string				sid_;
		std::string				type_;
		transformation_list_t	transformations_[3];
		float					matrix_[16];
		node					*parent_;
		node_list_t				children_;
		instance_controller		*instance_controller_;
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class transformation
	{
	public:
		enum E_TRANSFORMATION_TYPE
		{
			ETT_TRANSLATE,
			ETT_ROTATE,
			ETT_SCALE,
			ETT_COUNT
		};

		transformation(E_TRANSFORMATION_TYPE type) : type_(type) {}
		void deserialize(const TiXmlElement &element);

		E_TRANSFORMATION_TYPE	type_;
		std::string				sid_;
		float					values_[4];
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class instance_controller
	{
	public:
		void deserialize(const TiXmlElement &element);

		std::string	url_;
		std::string skeleton_;
	};
	//----------------------------------------------------------------------------------------------

} /*library*/ } /*collada*/

#endif /* COLLADA_LIBRARY_VISUAL_SCENES_HPP */