#ifndef DJAH_GAMEPLAY_COMPONENTS_PUNCH_CARD_HPP
#define DJAH_GAMEPLAY_COMPONENTS_PUNCH_CARD_HPP

#include "djah/debug/assertion.hpp"
#include "djah/core/typelist.hpp"
#include "djah/core/hierarchy_generation.hpp"
#include "djah/gameplay/component_database.hpp"

namespace djah { namespace gameplay {

	//==================================================================================================
	//--------------------------------------------------------------------------------------------------
	// Components punch card visitor declaration
	//--------------------------------------------------------------------------------------------------
	template<typename ComponentsTypeList>
	struct components_punch_card_visitor;

	//--------------------------------------------------------------------------------------------------
	// Components punch card visitor's stopping structure
	//--------------------------------------------------------------------------------------------------
	template<>
	struct components_punch_card_visitor<utils::nulltype>
	{
		//----------------------------------------------------------------------------------------------
		// Make a copy of the src punch card into the dst punch card.
		// Note that no clean up will be made, thus dst must be empty to have a proper clone. 
		//----------------------------------------------------------------------------------------------
        template<typename PunchCard>
        static void clone(const PunchCard &src, PunchCard &dst) {}

		//----------------------------------------------------------------------------------------------
		// Merge src container into dst punch card, i.e. dst will use every src component with the exact
		// same data.
		// Note that if the destination punch card already uses a given component, that component will
		// be overwritten with the source punch card's component data.
		//----------------------------------------------------------------------------------------------
        template<typename PunchCard>
        static void merge(const PunchCard &src, PunchCard &dst) {}

		//----------------------------------------------------------------------------------------------
		// Use the current component in the given punch card
		//----------------------------------------------------------------------------------------------
		template<typename PunchCard>
		static void use(PunchCard &punchCard) {}
		
		//----------------------------------------------------------------------------------------------
		// Remove every component from the given punch card
		//----------------------------------------------------------------------------------------------
		template<typename PunchCard>
		static void remove_all(PunchCard &punchCard) {}
		
		//----------------------------------------------------------------------------------------------
		// Is the given punch card using the current component
		// Stopping condition: if we got this far it means that we finished the type list thus
		// the punch card is indeed using all the components of ComponentsTypeList
		//----------------------------------------------------------------------------------------------
		template<typename PunchCard>
		static bool is_using(const PunchCard &punchCard) { return true; }
	};

	//--------------------------------------------------------------------------------------------------
	// Components punch card visitor functions actual implementation
	//--------------------------------------------------------------------------------------------------
	template<typename HeadComponent, typename TailList>
	struct components_punch_card_visitor< utils::typelist<HeadComponent,TailList> >
	{
        template<typename PunchCard>
        static void clone(const PunchCard &src, PunchCard &dst)
        {
            if( src.template isUsing<HeadComponent>() )
            {
                component<HeadComponent> srcComp = src.template get<HeadComponent>();
                dst.template use<HeadComponent>(*srcComp);
            }
            
            components_punch_card_visitor<TailList>::clone(src, dst);
        }
        
        template<typename PunchCard>
        static void merge(const PunchCard &src, PunchCard &dst)
        {
            if( src.template isUsing<HeadComponent>() )
            {
                const component<HeadComponent> &srcComp = src.template get<HeadComponent>();
                
                if( dst.template isUsing<HeadComponent>() )
                {
                    component<HeadComponent> dstComp = dst.template get<HeadComponent>();
                    *dstComp = *srcComp;
                }
                else
                {
                    dst.template use<HeadComponent>(*srcComp);
                }
            }
            
            components_punch_card_visitor<TailList>::merge(src, dst);
        }
        
		template<typename PunchCard>
		inline static void use(PunchCard &punchCard)
		{
			if( !punchCard.isUsing<HeadComponent>() )
			{
				punchCard.use<HeadComponent>();
			}
			components_punch_card_visitor<TailList>::use(punchCard);
		}

		template<typename PunchCard>
		inline static void remove_all(PunchCard &punchCard)
		{
			if( punchCard.isUsing<HeadComponent>() )
			{
				punchCard.stopUsing<HeadComponent>();
			}
			components_punch_card_visitor<TailList>::remove_all(punchCard);
		}

		template<typename PunchCard>
		inline static bool is_using(const PunchCard &punchCard)
		{
			return (punchCard.isUsing<HeadComponent>() && components_punch_card_visitor<TailList>::is_using(punchCard));
		}
	};


	//==================================================================================================
	//--------------------------------------------------------------------------------------------------
	// Keeps the index of the actual instance of ComponentType in the corresponding components_database
	// container.
	// An invalid index means that this particular ComponentType is not used by the components_punch_card.
	//--------------------------------------------------------------------------------------------------
	template<typename ComponentType>
	class component_usage
	{
	protected:
		component_usage()
			: cid_(INVALID_COMPONENT_ID)
		{}

	protected:
		// ID of this ComponentType instance in the component database
		ComponentID cid_;
	};

	//--------------------------------------------------------------------------------------------------
	// A component punch card, this class inherits from component_usage templated by each and every
	// component type in ComponentsTypeList. Basically it acts as a list of typed ids indexing an actual
	// container located in component_database.
	//
	// Example:
	//		components_punch_card< TYPELIST(Comp1, Comp2, ..., CompN) >
	//		will inherit from:
	//			- component_usage<Comp1>
	//			- component_usage<Comp2>
	//			- ...
	//			- component_usage<CompN>
	//--------------------------------------------------------------------------------------------------
	template<typename ComponentsTypeList>
	class components_punch_card
		: public utils::gen_scatter_hierarchy<ComponentsTypeList, component_usage>
	{
	public:
		//----------------------------------------------------------------------------------------------
        void clone(components_punch_card<ComponentsTypeList> &_clone) const
        {
            components_punch_card_visitor<ComponentsTypeList>::clone(*this, _clone);
        }
        
		//----------------------------------------------------------------------------------------------
        void merge(const components_punch_card<ComponentsTypeList> &src)
        {
            components_punch_card_visitor<ComponentsTypeList>::merge(src, *this);
        }

		//----------------------------------------------------------------------------------------------
		template<typename ComponentType>
		bool isUsing() const
		{
			return component_usage<ComponentType>::cid_ != INVALID_COMPONENT_ID;
		}

		//----------------------------------------------------------------------------------------------
		template<typename IsUsingComponents>
		bool isUsingList() const
		{
			return components_punch_card_visitor<IsUsingComponents>::is_using(*this);
		}

		//----------------------------------------------------------------------------------------------
		template<typename UseComponents>
		components_punch_card<ComponentsTypeList>& useList()
		{
			components_punch_card_visitor<UseComponents>::use(*this);
			return (*this);
		}

		//----------------------------------------------------------------------------------------------
		template<typename ComponentType>
		components_punch_card<ComponentsTypeList>& use(const ComponentType &comp = ComponentType())
		{
			check( !isUsing<ComponentType>() );
			component_usage<ComponentType>::cid_ = db_.add<ComponentType>(comp);
			check( isUsing<ComponentType>() );

			return (*this);
		}

		//----------------------------------------------------------------------------------------------
		template<typename ComponentType>
		components_punch_card<ComponentsTypeList>& stopUsing()
		{
			check( isUsing<ComponentType>() );

			db_.remove<ComponentType>(component_usage<ComponentType>::cid_);
			component_usage<ComponentType>::cid_ = INVALID_COMPONENT_ID;

			return (*this);
		}

		//----------------------------------------------------------------------------------------------
		void stopUsingAll()
		{
			components_punch_card_visitor<ComponentsTypeList>::remove_all(*this);
		}

		//----------------------------------------------------------------------------------------------
		template<typename ComponentType>
		inline component<ComponentType> get() const
		{
			check( isUsing<ComponentType>() );
			return db_.get<ComponentType>(component_usage<ComponentType>::cid_);
		}

	private:
		static component_database<ComponentsTypeList> db_;
	};
	//--------------------------------------------------------------------------------------------------
	template<typename ComponentTypeList>
	component_database<ComponentTypeList> components_punch_card<ComponentTypeList>::db_;
	//==================================================================================================

} /*gameplay*/ } /*djah*/

#endif /* DJAH_GAMEPLAY_COMPONENTS_PUNCH_CARD_HPP */