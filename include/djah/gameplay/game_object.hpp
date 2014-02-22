#ifndef DJAH_GAMEPLAY_GAME_OBJECT_HPP
#define DJAH_GAMEPLAY_GAME_OBJECT_HPP

#include "djah/debug/assertion.hpp"
#include "djah/core/typelist.hpp"
#include "djah/core/hierarchy_generation.hpp"
#include "djah/gameplay/component_database.hpp"
#include "djah/gameplay/component_serializer.hpp"

namespace djah { namespace gameplay {

	//--------------------------------------------------------------------------------------------------
	static const int NB_GO = 100;

	//==================================================================================================
	template<typename ComponentsTypeList>
	struct components_visitor;
	//--------------------------------------------------------------------------------------------------
	template<>
	struct components_visitor<utils::nulltype>
	{
		template<typename GO, typename DB>
		static void use(GO&, DB&) {}

		template<typename GO>
		static void remove_all(GO&) {}

		template<typename GO>
		static bool is_using(GO&) { return true; }
	};
	//--------------------------------------------------------------------------------------------------
	template<typename HeadComponent, typename TailList>
	struct components_visitor< utils::typelist<HeadComponent,TailList> >
	{
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
		inline static bool is_using(GO &go)
		{
			return (go.isUsing<HeadComponent>() && components_visitor<TailList>::is_using(go));
		}
	};
	//==================================================================================================


	//==================================================================================================
	template<typename ComponentType>
	class component_usage
	{
	public:
		component_usage()
			: cid_(INVALID_COMPONENT_ID)
			, pData_(nullptr)
		{}
		CID cid_;
		ComponentType *pData_;
	};
	//--------------------------------------------------------------------------------------------------
	template<typename ComponentsTypeList>
	class game_object
		: public utils::gen_scatter_hierarchy<ComponentsTypeList, component_usage>
	{
	public:
		typedef game_object_serializer<ComponentsTypeList> serializer_t;

	public:
		//----------------------------------------------------------------------------------------------
		game_object(const std::string &goName)
			: name_(goName)
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
			component_usage<ComponentType>::pData_ = &(get<ComponentType>().data());

			return (*this);
		}

		//----------------------------------------------------------------------------------------------
		template<typename ComponentType>
		game_object<ComponentsTypeList>& stopUsing()
		{
			check( isUsing<ComponentType>() );

			db_.remove<ComponentType>(component_usage<ComponentType>::cid_);
			component_usage<ComponentType>::cid_ = INVALID_COMPONENT_ID;
			component_usage<ComponentType>::pData_ = nullptr;

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
	};
	//--------------------------------------------------------------------------------------------------
	template<typename ComponentTypeList>
	component_database<ComponentTypeList> game_object<ComponentTypeList>::db_;
	//==================================================================================================

} /*gameplay*/ } /*djah*/

#endif /* DJAH_GAMEPLAY_GAME_OBJECT_HPP */