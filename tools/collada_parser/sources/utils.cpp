#include "utils.hpp"
#include "library/common.hpp"
#include "library/visual_scenes.hpp"
#include "library/controllers.hpp"
#include "library/animations.hpp"

using namespace collada::library;

namespace collada {

	//----------------------------------------------------------------------------------------------
	static const char *s_semantics_str[ESS_COUNT] =
	{
		"POSITION",
		"VERTEX",
		"NORMAL",

		"TEXCOORD",
		"TEXTANGENT",
		"TEXBINORMAL",

		"JOINT",
		"WEIGHT",
		"INV_BIND_MATRIX",

		"INPUT",
		"OUTPUT",
		"INTERPOLATION",
	};
	//----------------------------------------------------------------------------------------------

	const char* semantic_to_string(E_SOURCE_SEMANTIC semantic)
	{
		return s_semantics_str[semantic];
	}


	//----------------------------------------------------------------------------------------------
	input* get_input_by_semantic(E_SOURCE_SEMANTIC semantic, const input_list_t &inputs, unsigned int _set)
	{
		const char *sem_str = s_semantics_str[semantic];
		input *ipt = nullptr;

		input_list_t::const_iterator it;
		input_list_t::const_iterator it_end = inputs.end();
		for(it = inputs.begin(); it != it_end && ipt == nullptr; ++it)
		{
			if( ((*it)->semantic_ == sem_str) && ((*it)->set_ == _set) )
				ipt = (*it);
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
		source *src = nullptr;

		if( !source_id.empty() )
		{
			source_list_t::const_iterator it;
			source_list_t::const_iterator it_end = sources.end();
			for(it = sources.begin(); it != it_end && src == nullptr; ++it)
			{
				if((*it)->id_ == source_id)
					src = (*it);
			}
		}

		return src;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	source* get_source_by_semantic(E_SOURCE_SEMANTIC semantic, const input_list_t &inputs, const source_list_t &sources)
	{
		const std::string &source_id = get_source_id_by_semantic(semantic, inputs);
		return get_source_by_id(source_id, sources);		
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	node* get_node_by_sid(const std::string &node_sid, const node_list_t &nodes)
	{
		node *n = nullptr;

		if( !node_sid.empty() )
		{
			// Current level
			node_list_t::const_iterator it;
			node_list_t::const_iterator it_end = nodes.end();
			for(it = nodes.begin(); it != it_end && n == nullptr; ++it)
			{
				if((*it)->sid_ == node_sid)
					n = (*it);
			}

			// Search in children
			if( !n )
			{
				for(it = nodes.begin(); it != it_end && n == nullptr; ++it)
					n = get_node_by_sid(node_sid, (*it)->children_);
			}
		}

		return n;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	controller* get_controller_by_skin_source(const std::string &skin_source, const controller_list_t &ctrlers)
	{
		controller *ctrler = nullptr;

		controller_list_t::const_iterator it;
		controller_list_t::const_iterator it_end = ctrlers.end();
		for(it = ctrlers.begin(); it != it_end && ctrler == nullptr; ++it)
		{
			if( (*it)->skin_ && (*it)->skin_->source_ == skin_source )
				ctrler = (*it);
		}

		return ctrler;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	animation_list_t get_animations_by_target_id(const std::string &target_id, const animation_list_t &anims)
	{
		animation_list_t anims_found;

		animation_list_t::const_iterator it;
		animation_list_t::const_iterator it_end = anims.end();
		for(it = anims.begin(); it != it_end; ++it)
		{
			if( (*it)->id_.find(target_id) != std::string::npos )
			{
				const channel_list_t &channels = (*it)->channels_;
				channel_list_t::const_iterator ch_it;
				channel_list_t::const_iterator ch_it_end = channels.end();
				for(ch_it = channels.begin(); ch_it != ch_it_end; ++ch_it)
				{
					int pos = (*ch_it)->target_.find_last_of("/");
					const std::string &target = (*ch_it)->target_.substr(0,pos);
					if( target == target_id )
					{
						anims_found.push_back( *it );
						break;
					}
				}
			}
		}

		return anims_found;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	sampler* get_sampler_by_id(const std::string &sampler_id, const sampler_list_t &samplers)
	{
		sampler *spl = nullptr;

		sampler_list_t::const_iterator it;
		sampler_list_t::const_iterator it_end = samplers.end();
		for(it = samplers.begin(); it != it_end && spl == nullptr; ++it)
		{
			if( (*it)->id_ == sampler_id )
				spl = (*it);
		}

		return spl;
	}
	//----------------------------------------------------------------------------------------------

} /*collada*/