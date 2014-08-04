#ifndef DJAH_GAMEPLAY_COMPONENT_DATABASE_HPP
#define DJAH_GAMEPLAY_COMPONENT_DATABASE_HPP

#include <stack>
#include <vector>
#include "djah/types.hpp"
#include "djah/core/hierarchy_generation.hpp"
#include "djah/debug/assertion.hpp"

namespace djah { namespace gameplay {

	typedef u16 CID;
	enum { INVALID_COMPONENT_ID = CID(-1) };

	//==================================================================================================
	template<typename ComponentType>
	struct component_holder;
	//--------------------------------------------------------------------------------------------------
	template<typename ComponentType>
	class component
	{
		friend struct component_holder<ComponentType>;

	public:
		struct component_data
		{
			u32 creationTimestamp;
			ComponentType componentData;
		};
		typedef std::vector<component_data> component_list_t;

	public:
		inline ComponentType& data()
		{
			check(components_[cid_].creationTimestamp == handlerTimestamp_);
			return components_[cid_].componentData;
		}
		inline ComponentType& operator *()	{ return data(); }
		inline ComponentType* operator->()	{ return &data(); }

		inline const ComponentType& data() const
		{
			check(components_[cid_].creationTimestamp == handlerTimestamp_);
			return components_[cid_].componentData;
		}
		inline const ComponentType& operator *() const 	{ return data(); }
		inline const ComponentType* operator->() const 	{ return &data(); }

	private:
		component(CID cid, component_list_t &components)
			: cid_(cid)
			, components_(components)
			, handlerTimestamp_(components[cid].creationTimestamp)
		{}

	private:
		CID cid_;
		component_list_t &components_;
		u32 handlerTimestamp_;
	};
	//==================================================================================================


	//==================================================================================================
	template<typename ComponentType>
	struct component_holder
	{
		component_holder()
		{
			components_.reserve(ComponentType::NB_COMP);
		}

		inline component<ComponentType> get(CID cid)
		{
			check(cid < components_.size());
			check(components_[cid].creationTimestamp != 0);

			return component<ComponentType>(cid, components_);
		}

		typedef typename component<ComponentType>::component_list_t	component_list_t;
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

			typename component<ComponentType>::component_data compData =
			{
				static_cast<u32>(::time(nullptr)),
				comp
			};

			if( component_holder<ComponentType>::freeSpots_.empty() )
			{
				component_holder<ComponentType>::components_.push_back( compData );
				cid = component_holder<ComponentType>::components_.size() - 1;
			}
			else
			{
				cid = component_holder<ComponentType>::freeSpots_.top();
				component_holder<ComponentType>::freeSpots_.pop();
				component_holder<ComponentType>::components_[cid] = compData;
			}

			return cid;
		}

		template<typename ComponentType>
		inline void remove(CID cid)
		{
			typename component<ComponentType>::component_data compData = { 0, ComponentType() };

			check(cid < component_holder<ComponentType>::components_.size());
			component_holder<ComponentType>::components_[cid] = compData;
			component_holder<ComponentType>::freeSpots_.push(cid);
		}
	};

} /*gameplay*/ } /*djah*/

#endif /* DJAH_GAMEPLAY_COMPONENT_DATABASE_HPP */