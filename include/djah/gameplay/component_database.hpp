#ifndef DJAH_GAMEPLAY_COMPONENT_DATABASE_HPP
#define DJAH_GAMEPLAY_COMPONENT_DATABASE_HPP

#include <stack>
#include "djah/core/hierarchy_generation.hpp"

namespace djah { namespace gameplay {

	typedef unsigned short CID;
	enum { INVALID_COMPONENT_ID = CID(-1) };

	//==================================================================================================
	template<typename ComponentType>
	struct component_holder;
	//--------------------------------------------------------------------------------------------------
	template<typename ComponentType>
	class component
	{
		friend struct component_holder<ComponentType>;
		typedef std::vector<ComponentType> component_list_t;

	public:
		inline operator ComponentType() { return components_[cid_]; }
		inline ComponentType& data() { return components_[cid_]; }

	private:
		inline component(CID cid, component_list_t &components)
			: cid_(cid)
			, components_(components)
		{}
		CID cid_;
		component_list_t &components_;
	};
	//==================================================================================================


	//==================================================================================================
	template<typename ComponentType>
	struct component_holder
	{
		component_holder()
		{
			components_.reserve(NB_GO);
		}

		inline component<ComponentType> get(CID cid)
		{
			assert(cid < components_.size());
			return component<ComponentType>(cid, components_);
		}

		typedef std::vector<ComponentType> component_list_t;
		component_list_t	components_;
		std::stack<CID>		freeSpots_;
	};
	//--------------------------------------------------------------------------------------------------
	template<typename ComponentTypeList>
	class component_database
		: public utils::gen_scatter_hierarchy<ComponentTypeList, component_holder>
	{
	public:
		template<typename ComponentType>
		inline component<ComponentType> get(CID cid)
		{
			return component_holder<ComponentType>::get(cid);
		}

		template<typename ComponentType>
		inline CID add(const ComponentType &comp)
		{
			CID cid = INVALID_COMPONENT_ID;

			if( component_holder<ComponentType>::freeSpots_.empty() )
			{
				component_holder<ComponentType>::components_.push_back( comp );
				cid = component_holder<ComponentType>::components_.size() - 1;
			}
			else
			{
				cid = component_holder<ComponentType>::freeSpots_.top();
				component_holder<ComponentType>::freeSpots_.pop();
				component_holder<ComponentType>::components_[cid] = comp;
			}

			return cid;
		}

		template<typename ComponentType>
		inline void remove(CID cid)
		{
			assert(cid < component_holder<ComponentType>::components_.size());
			component_holder<ComponentType>::components_[cid] = ComponentType();
			component_holder<ComponentType>::freeSpots_.push(cid);
		}
	};

} /*gameplay*/ } /*djah*/

#endif /* DJAH_GAMEPLAY_COMPONENT_DATABASE_HPP */