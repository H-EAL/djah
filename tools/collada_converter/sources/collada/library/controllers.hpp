#ifndef COLLADA_LIBRARY_CONTROLLERS_HPP
#define COLLADA_LIBRARY_CONTROLLERS_HPP

#include "../fwd_decl.hpp"

namespace collada { namespace library {

	//----------------------------------------------------------------------------------------------
	class skin;
	class joints;
	class vertex_weights;
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	class controllers
	{
	public:
		void deserialize(const TiXmlElement &element);
		~controllers();

		controller_list_t controllers_;
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class controller
	{
	public:
		controller() : skin_(0) {}
		void deserialize(const TiXmlElement &element);
		~controller();

		std::string	id_;
		std::string name_;
		skin		*skin_;
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class skin
	{
	public:
		skin() : joints_(0), vertex_weights_(0) {}
		void deserialize(const TiXmlElement &element);
		~skin();

		std::string		source_;
		float			bind_shape_matrix_[16];
		source_list_t	sources_;
		joints			*joints_;
		vertex_weights	*vertex_weights_;
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class joints
	{
	public:
		void deserialize(const TiXmlElement &element);
		~joints();

		input_list_t inputs_;
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class vertex_weights
	{
	public:
		vertex_weights() : vcount_(0), v_(0) {}
		void deserialize(const TiXmlElement &element);
		~vertex_weights();

		unsigned int	count_;
		input_list_t	inputs_;
		unsigned short	*vcount_;
		unsigned short	*v_;
	};
	//----------------------------------------------------------------------------------------------

} /*library*/ } /*collada*/

#endif /* COLLADA_LIBRARY_CONTROLLERS_HPP */