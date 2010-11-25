#ifndef COLLADA_UTILS_HPP
#define COLLADA_UTILS_HPP

#include "fwd_decl.hpp"

namespace collada {

	//----------------------------------------------------------------------------------------------
	enum E_SOURCE_SEMANTIC
	{
		ESS_POSITION,
		ESS_VERTEX,
		ESS_NORMAL,
		ESS_TEXCOORD,

		ESS_JOINT,
		ESS_WEIGHT,
		ESS_INV_BIND_MATRIX,

		ESS_COUNT
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	library::input* get_input_by_semantic(	E_SOURCE_SEMANTIC semantic,
											const library::input_list_t &inputs);
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	std::string	get_source_id_by_semantic(	E_SOURCE_SEMANTIC semantic,
											const library::input_list_t &inputs);
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	library::source* get_source_by_id(		const std::string &source_id,
											const library::source_list_t &sources);
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	library::source* get_source_by_semantic(E_SOURCE_SEMANTIC semantic,
											const library::input_list_t &inputs,
											const library::source_list_t &sources);
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	std::string find_root_node_id(const library::visual_scene &visualscene);
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	library::node* find_node_by_id(const library::visual_scene &visualscene, const std::string &node_id);
	//----------------------------------------------------------------------------------------------


} /*collada*/

#endif /* COLLADA_UTILS_HPP */