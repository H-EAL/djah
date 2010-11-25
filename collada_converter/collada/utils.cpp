#include "utils.hpp"
#include "library/common.hpp"
#include "library/visual_scenes.hpp"

using namespace collada::library;

namespace collada {

	//----------------------------------------------------------------------------------------------
	static const char *s_semantics_str[ESS_COUNT] =
	{
		"POSITION",
		"VERTEX",
		"NORMAL",
		"TEXCOORD",

		"JOINT",
		"WEIGHT",
		"INV_BIND_MATRIX"
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	input* get_input_by_semantic(E_SOURCE_SEMANTIC semantic, const input_list_t &inputs)
	{
		const char *sem_str = s_semantics_str[semantic];
		input *ipt = 0;

		input_list_t::const_iterator it;
		input_list_t::const_iterator it_end = inputs.end();
		for(it = inputs.begin(); it != it_end; ++it)
		{
			if((*it)->semantic_ == sem_str)
			{
				ipt = (*it);
				break;
			}
		}

		return ipt;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	std::string	get_source_id_by_semantic(E_SOURCE_SEMANTIC semantic, const input_list_t &inputs)
	{
		input *ipt = get_input_by_semantic(semantic, inputs);
		return ipt ? ipt->source_ : "";
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	source* get_source_by_id(const std::string &source_id, const source_list_t &sources)
	{
		source *src = 0;

		if( !source_id.empty() )
		{
			source_list_t::const_iterator it;
			source_list_t::const_iterator it_end = sources.end();
			for(it = sources.begin(); it != it_end && src == 0; ++it)
				if((*it)->id_ == source_id)
					src = (*it);
		}

		return src;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	source* get_source_by_semantic(E_SOURCE_SEMANTIC semantic, const input_list_t &inputs, const source_list_t &sources)
	{
		std::string source_id = get_source_id_by_semantic(semantic, inputs);
		return get_source_by_id(source_id, sources);		
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	node* find_node_by_id(const visual_scene &visualscene, const std::string &node_id)
	{
		node *n = 0;

		if(!node_id.empty())
		{
			node_list_t::const_iterator it;
			node_list_t::const_iterator it_end = visualscene.nodes_.end();
			for(it = visualscene.nodes_.begin(); it != it_end && n == 0; ++it)
				if((*it)->id_ == node_id)
					n = (*it);
		}

		return n;
	}
	//----------------------------------------------------------------------------------------------

} /*collada*/