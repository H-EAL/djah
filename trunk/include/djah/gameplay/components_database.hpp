#ifndef DJAH_GAMEPLAY_COMPONENTS_DATABASE_HPP
#define DJAH_GAMEPLAY_COMPONENTS_DATABASE_HPP

#include <stack>
#include <vector>
#include "djah/types.hpp"
#include "djah/core/hierarchy_generation.hpp"
#include "djah/debug/assertion.hpp"

namespace djah { namespace gameplay {

	typedef u16 ComponentID;
	enum { INVALID_COMPONENT_ID = ComponentID(-1) };

	//==================================================================================================
	template<typename ComponentType>
	struct components_container;
	//--------------------------------------------------------------------------------------------------
	template<typename ComponentType>
	class component
	{
		friend struct components_container<ComponentType>;

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
		component(ComponentID cid, component_list_t &components)
			: cid_(cid)
			, components_(components)
			, handlerTimestamp_(components[cid].creationTimestamp)
		{}

	private:
		ComponentID cid_;
		component_list_t &components_;
		u32 handlerTimestamp_;
	};
	//==================================================================================================


	//==================================================================================================
	template<typename ComponentType>
	struct components_container
	{
		components_container()
		{
			components_.reserve(ComponentType::NB_COMP);
		}

		inline component<ComponentType> get(ComponentID cid)
		{
			check(cid < components_.size());
			check(components_[cid].creationTimestamp != 0);

			return component<ComponentType>(cid, components_);
		}

		typedef typename component<ComponentType>::component_list_t	component_list_t;
		component_list_t		components_;
		std::stack<ComponentID>	freeSpots_;
	};

	//--------------------------------------------------------------------------------------------------
	// Restrictions on ComponentsTypeList types are that they implement the functions and constructors
	// defined in the MAKE_COMPONENT macro (cf. djah/gameplay/component.hpp) and that they expose at
	// least copy semantics and at best move semantics.
	//--------------------------------------------------------------------------------------------------
	template<typename ComponentsTypeList>
	class components_database
		: public utils::gen_scatter_hierarchy<ComponentsTypeList, components_container>
	{
	public:
		template<typename ComponentType>
		inline component<ComponentType> get(ComponentID cid)
		{
			return components_container<ComponentType>::get(cid);
		}

		template<typename ComponentType>
		inline ComponentID add(const ComponentType &comp)
		{
			ComponentID cid = INVALID_COMPONENT_ID;

			typename component<ComponentType>::component_data compData =
			{
				static_cast<u32>(::time(nullptr)),
				comp
			};

			if( components_container<ComponentType>::freeSpots_.empty() )
			{
				components_container<ComponentType>::components_.push_back( compData );
				cid = components_container<ComponentType>::components_.size() - 1;
			}
			else
			{
				cid = components_container<ComponentType>::freeSpots_.top();
				components_container<ComponentType>::freeSpots_.pop();
				components_container<ComponentType>::components_[cid] = compData;
			}

			return cid;
		}

		template<typename ComponentType>
		inline void remove(ComponentID cid)
		{
			typename component<ComponentType>::component_data compData = { 0, ComponentType() };

			check(cid < components_container<ComponentType>::components_.size());
			components_container<ComponentType>::components_[cid] = compData;
			components_container<ComponentType>::freeSpots_.push(cid);
		}
	};

} /*gameplay*/ } /*djah*/

#endif /* DJAH_GAMEPLAY_COMPONENTS_DATABASE_HPP */