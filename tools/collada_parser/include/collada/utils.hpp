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
		ESS_TEXTANGENT,
		ESS_TEXBINORMAL,

		ESS_JOINT,
		ESS_WEIGHT,
		ESS_INV_BIND_MATRIX,

		ESS_INPUT,
		ESS_OUTPUT,
		ESS_INTERPOLATION,

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
	library::node* get_node_by_sid(			const std::string &node_sid,
											const library::node_list_t &nodes);
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	library::controller* get_controller_by_skin_source(	const std::string &skin_source,
														const library::controller_list_t &ctrlers);
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	library::animation_list_t get_animations_by_target_id(	const std::string &target_id,
															const library::animation_list_t &anims);
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	library::sampler* get_sampler_by_id(	const std::string &sampler_id,
											const library::sampler_list_t &samplers);
	//----------------------------------------------------------------------------------------------
	
} /*collada*/

#endif /* COLLADA_UTILS_HPP */