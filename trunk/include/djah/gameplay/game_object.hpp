#ifndef DJAH_GAMEPLAY_GAME_OBJECT_HPP
#define DJAH_GAMEPLAY_GAME_OBJECT_HPP

#include "djah/debug/assertion.hpp"
#include "djah/core/typelist.hpp"
#include "djah/core/hierarchy_generation.hpp"
#include "djah/gameplay/component_database.hpp"

namespace djah { namespace gameplay {

	//==================================================================================================
	template<typename ComponentsTypeList>
	struct components_visitor;
	//--------------------------------------------------------------------------------------------------
	template<>
	struct components_visitor<utils::nulltype>
	{
        template<typename GO>
        static void clone(const GO&, GO&) {}
        
        template<typename GO>
        static void merge(GO&, const GO&) {}
        
		template<typename GO>
		static void use(GO&) {}

		template<typename GO>
		static void remove_all(GO&) {}

		template<typename GO>
		static bool is_using(const GO&) { return true; }
	};
	//--------------------------------------------------------------------------------------------------
	template<typename HeadComponent, typename TailList>
	struct components_visitor< utils::typelist<HeadComponent,TailList> >
	{
        template<typename GO>
        inline static void clone(const GO &srcGo, GO &dstGo)
        {
            if( srcGo.template isUsing<HeadComponent>() )
            {
                component<HeadComponent> comp = srcGo.template get<HeadComponent>();
                dstGo.template use<HeadComponent>(*comp);
            }
            
            components_visitor<TailList>::clone(srcGo, dstGo);
        }
        
        template<typename GO>
        inline static void merge(GO &mergeInto, const GO &mergeWith)
        {
            if( mergeWith.template isUsing<HeadComponent>() )
            {
                component<HeadComponent> compB = mergeWith.template get<HeadComponent>();
                
                if( mergeInto.template isUsing<HeadComponent>() )
                {
                    component<HeadComponent> compA = mergeInto.template get<HeadComponent>();
                    *compA = *compB;
                }
                else
                {
                    mergeInto.template use<HeadComponent>(*compB);
                }
            }
            
            components_visitor<TailList>::merge(mergeInto, mergeWith);
        }
        
		template<typename GO>
		inline static void use(GO &go)
		{
			if( !go.isUsing<HeadComponent>() )
			{
				go.use<HeadComponent>();
			}
			components_visitor<TailList>::use(go);
		}

		template<typename GO>
		inline static void remove_all(GO &go)
		{
			if( go.isUsing<HeadComponent>() )
			{
				go.stopUsing<HeadComponent>();
			}
			components_visitor<TailList>::remove_all(go);
		}

		template<typename GO>
		inline static bool is_using(const GO &go)
		{
			return (go.isUsing<HeadComponent>() && components_visitor<TailList>::is_using(go));
		}
	};
	//==================================================================================================


	//==================================================================================================
	template<typename ComponentType>
	struct component_usage
	{
		component_usage() : cid_(INVALID_COMPONENT_ID) {}
		CID cid_;
	};
	//--------------------------------------------------------------------------------------------------
	template<typename ComponentsTypeList>
	class game_object
		: public utils::gen_scatter_hierarchy<ComponentsTypeList, component_usage>
	{
	public:
		//----------------------------------------------------------------------------------------------
		game_object(const std::string &goName = "unnamed_gameobject")
			: name_(goName)
        	, enabled_(true)
		{
		}

		//----------------------------------------------------------------------------------------------
		~game_object()
		{
			stopUsingAll();
		}

		//----------------------------------------------------------------------------------------------
		const std::string& name() const
		{
			return name_;
		}
        
		//----------------------------------------------------------------------------------------------
        std::string& name()
		{
			return name_;
		}
        
		//----------------------------------------------------------------------------------------------
        bool isEnabled() const
        {
            return enabled_;
        }
        
		//----------------------------------------------------------------------------------------------
        void setEnabled(bool enabled)
        {
            enabled_ = enabled;
        }
        
		//----------------------------------------------------------------------------------------------
        void enable()
        {
            setEnabled(true);
        }
        
		//----------------------------------------------------------------------------------------------
        void disable()
        {
            setEnabled(false);
        }
        
		//----------------------------------------------------------------------------------------------
        game_object<ComponentsTypeList> clone(const std::string &cloneName = "") const
        {
            game_object<ComponentsTypeList> clonedGo(cloneName.empty() ? name_ + "_clone" : cloneName);
            components_visitor<ComponentsTypeList>::clone(*this, clonedGo);
            return clonedGo;
        }
        
		//----------------------------------------------------------------------------------------------
        void merge(const game_object<ComponentsTypeList> &other)
        {
            components_visitor<ComponentsTypeList>::merge(*this, other);
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
			return components_visitor<IsUsingComponents>::is_using(*this);
		}

		//----------------------------------------------------------------------------------------------
		template<typename UseComponents>
		game_object<ComponentsTypeList>& useList()
		{
			components_visitor<UseComponents>::use(*this);
			return (*this);
		}

		//----------------------------------------------------------------------------------------------
		template<typename ComponentType>
		game_object<ComponentsTypeList>& use(const ComponentType &comp = ComponentType())
		{
			check( !isUsing<ComponentType>() );
			component_usage<ComponentType>::cid_ = db_.add<ComponentType>(comp);
			check( isUsing<ComponentType>() );

			return (*this);
		}

		//----------------------------------------------------------------------------------------------
		template<typename ComponentType>
		game_object<ComponentsTypeList>& stopUsing()
		{
			check( isUsing<ComponentType>() );

			db_.remove<ComponentType>(component_usage<ComponentType>::cid_);
			component_usage<ComponentType>::cid_ = INVALID_COMPONENT_ID;

			return (*this);
		}

		//----------------------------------------------------------------------------------------------
		void stopUsingAll()
		{
			components_visitor<ComponentsTypeList>::remove_all(*this);
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
		std::string name_;
        bool enabled_;
	};
	//--------------------------------------------------------------------------------------------------
	template<typename ComponentTypeList>
	component_database<ComponentTypeList> game_object<ComponentTypeList>::db_;
	//==================================================================================================

} /*gameplay*/ } /*djah*/

#endif /* DJAH_GAMEPLAY_GAME_OBJECT_HPP */