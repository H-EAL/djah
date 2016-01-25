#ifndef DJAH_GAMEPLAY_COMPONENTS_PUNCH_CARD_HPP
#define DJAH_GAMEPLAY_COMPONENTS_PUNCH_CARD_HPP

#include "djah/debug/assertion.hpp"
#include "djah/core/typelist.hpp"
#include "djah/core/hierarchy_generation.hpp"
#include "djah/gameplay/components_database.hpp"

namespace djah { namespace gameplay {

    //--------------------------------------------------------------------------------------------------
    // Components punch card visitor declaration
    //--------------------------------------------------------------------------------------------------
    template<typename ComponentsTypeList>
    struct components_punch_card_visitor;

    //--------------------------------------------------------------------------------------------------
    // Components punch card visitor's stopping structure
    //--------------------------------------------------------------------------------------------------
    template<>
    struct components_punch_card_visitor<nulltype>
    {
        //----------------------------------------------------------------------------------------------
        // Moves ownership of components from src punch card to dst punch card
        //----------------------------------------------------------------------------------------------
        template<typename PunchCard>
        static void move(PunchCard &src, PunchCard &dst) {}

        //----------------------------------------------------------------------------------------------
        // Make a copy of the src punch card into the dst punch card.
        // Note that no clean up will be made, thus dst must be empty to have a proper clone. 
        //----------------------------------------------------------------------------------------------
        template<typename PunchCard>
        static void clone(const PunchCard &src, PunchCard &dst) {}

        //----------------------------------------------------------------------------------------------
        // Merge src punch card into dst punch card, i.e. dst will use every src component with the
        // exact same data.
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
    struct components_punch_card_visitor< typelist<HeadComponent,TailList> >
    {
        //----------------------------------------------------------------------------------------------
        template<typename PunchCard>
        static void move(PunchCard &src, PunchCard &dst)
        {
            if( src.template isUsing<HeadComponent>() )
            {
                src.moveTo<HeadComponent>(dst);
            }

            components_punch_card_visitor<TailList>::move(src, dst);
        }

        //----------------------------------------------------------------------------------------------
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

        //----------------------------------------------------------------------------------------------
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

        //----------------------------------------------------------------------------------------------
        template<typename PunchCard>
        inline static void use(PunchCard &punchCard)
        {
            if( !punchCard.isUsing<HeadComponent>() )
            {
                punchCard.use<HeadComponent>();
            }
            components_punch_card_visitor<TailList>::use(punchCard);
        }

        //----------------------------------------------------------------------------------------------
        template<typename PunchCard>
        inline static void remove_all(PunchCard &punchCard)
        {
            if( punchCard.isUsing<HeadComponent>() )
            {
                punchCard.stopUsing<HeadComponent>();
            }
            components_punch_card_visitor<TailList>::remove_all(punchCard);
        }

        //----------------------------------------------------------------------------------------------
        template<typename PunchCard>
        inline static bool is_using(const PunchCard &punchCard)
        {
            return (punchCard.isUsing<HeadComponent>() && components_punch_card_visitor<TailList>::is_using(punchCard));
        }
    };


    //--------------------------------------------------------------------------------------------------
    // Keeps the index of the actual instance of ComponentType in the corresponding components_database
    // container.
    // An invalid index means that this particular ComponentType is not used by the components_punch_card.
    //--------------------------------------------------------------------------------------------------
    template<typename ComponentType>
    class component_index
    {
    protected:
        // ID of this ComponentType instance in the components database
        ComponentID cid_ = INVALID_COMPONENT_ID;
    };

    //--------------------------------------------------------------------------------------------------
    // A components punch card, this class inherits from component_index templated by each and every
    // component type in ComponentsTypeList. Basically it acts as a list of typed ids indexing an actual
    // container located in components_database.
    //
    // Example:
    //		components_punch_card< TYPELIST(Comp1, Comp2, ..., CompN) >
    //		will inherit from:
    //			- component_index<Comp1>
    //			- component_index<Comp2>
    //			- ...
    //			- component_index<CompN>
    //
    // Visually a punch card could look like this:
    //		_______________________________________________________________
    //		| ComponentsPunchCard | Comp1 | Comp2 | Comp3 | ..... | CompN |
    //		---------------------------------------------------------------
    //		| Component Index     |  428  |   /   |  33   | ..... |   9   |
    //		---------------------------------------------------------------
    //
    //--------------------------------------------------------------------------------------------------
    template<template<typename> class DataBase, typename ComponentsTypeList>
    class components_punch_card
        : public gen_scatter_hierarchy<ComponentsTypeList, component_index>
    {
    public:
        //----------------------------------------------------------------------------------------------
        components_punch_card() = default;

        //----------------------------------------------------------------------------------------------
        components_punch_card(const components_punch_card<DataBase, ComponentsTypeList> &src) = delete;

        //----------------------------------------------------------------------------------------------
        components_punch_card<DataBase, ComponentsTypeList>& operator =(const components_punch_card<DataBase, ComponentsTypeList> &rhs) = delete;

        //----------------------------------------------------------------------------------------------
        components_punch_card(components_punch_card<DataBase, ComponentsTypeList> &&src)
        {
            components_punch_card_visitor<ComponentsTypeList>::move(src, *this);
        }

        //----------------------------------------------------------------------------------------------
        components_punch_card<DataBase, ComponentsTypeList>& operator =(components_punch_card<DataBase, ComponentsTypeList> &&rhs)
        {
            if (this != &rhs)
            {
                components_punch_card_visitor<ComponentsTypeList>::move(rhs, *this);
            }
            return (*this);
        }

        //----------------------------------------------------------------------------------------------
        ~components_punch_card()
        {
            stopUsingAll();
        }

        //----------------------------------------------------------------------------------------------
        void clone(components_punch_card<DataBase, ComponentsTypeList> &dst) const
        {
            components_punch_card_visitor<ComponentsTypeList>::clone(*this, dst);
        }
        
        //----------------------------------------------------------------------------------------------
        void merge(const components_punch_card<DataBase, ComponentsTypeList> &src)
        {
            components_punch_card_visitor<ComponentsTypeList>::merge(src, *this);
        }

        //----------------------------------------------------------------------------------------------
        template<typename ComponentType>
        bool isUsing() const
        {
            return component_index<ComponentType>::cid_ != INVALID_COMPONENT_ID;
        }

        //----------------------------------------------------------------------------------------------
        template<typename IsUsingComponents>
        bool isUsingList() const
        {
            return components_punch_card_visitor<IsUsingComponents>::is_using(*this);
        }

        //----------------------------------------------------------------------------------------------
        template<typename UseComponents>
        components_punch_card<DataBase, ComponentsTypeList>& useList()
        {
            components_punch_card_visitor<UseComponents>::use(*this);
            return (*this);
        }

        //----------------------------------------------------------------------------------------------
        template<typename ComponentType>
        components_punch_card<DataBase, ComponentsTypeList>& use(const ComponentType &comp = ComponentType())
        {
            check( !isUsing<ComponentType>() );
            component_index<ComponentType>::cid_ = db().add<ComponentType>(comp);
            check( isUsing<ComponentType>() );

            return (*this);
        }

        //----------------------------------------------------------------------------------------------
        template<typename ComponentType>
        components_punch_card<DataBase, ComponentsTypeList>& stopUsing()
        {
            check( isUsing<ComponentType>() );

            db().remove<ComponentType>(component_index<ComponentType>::cid_);
            component_index<ComponentType>::cid_ = INVALID_COMPONENT_ID;

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
            return db().get<ComponentType>(component_index<ComponentType>::cid_);
        }

        //----------------------------------------------------------------------------------------------
        template<typename ComponentType>
        void moveTo(components_punch_card<DataBase, ComponentsTypeList> &dst)
        {
            dst.component_index<ComponentType>::cid_ = component_index<ComponentType>::cid_;
            component_index<ComponentType>::cid_ = INVALID_COMPONENT_ID;
        }

        //----------------------------------------------------------------------------------------------
        template<typename ComponentType>
        inline ComponentID getID() const
        {
            return component_index<ComponentType>::cid_;
        }

    private:
        //----------------------------------------------------------------------------------------------
        static inline components_database<ComponentsTypeList>& db()
        {
            static DataBase<ComponentsTypeList> sDB;
            return sDB;
            //return components_database<ComponentsTypeList>::get();
        }
    };

} /*gameplay*/ } /*djah*/

#endif /* DJAH_GAMEPLAY_COMPONENTS_PUNCH_CARD_HPP */