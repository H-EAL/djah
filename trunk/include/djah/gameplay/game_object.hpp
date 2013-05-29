#ifndef DJAH_GAMEPLAY_GAME_OBJECT_HPP
#define DJAH_GAMEPLAY_GAME_OBJECT_HPP

#include "djah/debug/assertion.hpp"
#include "djah/core/typelist.hpp"
#include "djah/core/hierarchy_generation.hpp"
#include "djah/gameplay/component_database.hpp"
#include "djah/gameplay/component_serializer.hpp"

namespace djah { namespace gameplay {

	//--------------------------------------------------------------------------------------------------
	static const int NB_GO = 1 << 10;

	//==================================================================================================
	template<typename ComponentTypeList>
	struct component_hook;
	//--------------------------------------------------------------------------------------------------
	template<>
	struct component_hook<utils::nulltype>
	{
		template<typename GO, typename DB>
		static void use(GO&, DB&) {}

		template<typename GO, typename DB>
		static void remove_all(GO&, DB&) {}

		template<typename GO>
		static bool is_using(GO&) { return true; }
	};
	//--------------------------------------------------------------------------------------------------
	template<typename HeadComponent, typename TailList>
	struct component_hook< utils::typelist<HeadComponent,TailList> >
	{
		template<typename GO, typename DB>
		inline static void use(GO &go, DB &db)
		{
			if( !go.isUsing<HeadComponent>() )
			{
				go.use<HeadComponent>();
			}
			component_hook<TailList>::use(go, db);
		}

		template<typename GO, typename DB>
		inline static void remove_all(GO &go, DB &db)
		{
			if( go.isUsing<HeadComponent>() )
			{
				go.stopUsing<HeadComponent>();
			}
			component_hook<TailList>::remove_all(go, db);
		}

		template<typename GO>
		inline static bool is_using(GO &go)
		{
			return (go.isUsing<HeadComponent>() && component_hook<TailList>::is_using(go));
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
	template<typename ComponentTypeList>
	class game_object
		: public utils::gen_scatter_hierarchy<ComponentTypeList, component_usage>
	{
	public:
		typedef game_object_serializer<ComponentTypeList> serializer_t;

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
			return component_hook<IsUsingComponents>::is_using(*this);
		}

		//----------------------------------------------------------------------------------------------
		template<typename UseComponents>
		game_object<ComponentTypeList>& useList()
		{
			component_hook<UseComponents>::use(*this, db_);
			return (*this);
		}

		//----------------------------------------------------------------------------------------------
		template<typename ComponentType>
		game_object<ComponentTypeList>& use(const ComponentType &comp = ComponentType())
		{
			DJAH_ASSERT( !isUsing<ComponentType>() );
			component_usage<ComponentType>::cid_ = db_.add<ComponentType>(comp);
			DJAH_ASSERT( isUsing<ComponentType>() );
			component_usage<ComponentType>::pData_ = &get<ComponentType>();

			return (*this);
		}

		//----------------------------------------------------------------------------------------------
		template<typename ComponentType>
		game_object<ComponentTypeList>& stopUsing()
		{
			DJAH_ASSERT( isUsing<ComponentType>() );

			db_.remove<ComponentType>(component_usage<ComponentType>::cid_);
			component_usage<ComponentType>::cid_ = INVALID_COMPONENT_ID;
			component_usage<ComponentType>::pData_ = nullptr;

			return (*this);
		}

		//----------------------------------------------------------------------------------------------
		void stopUsingAll()
		{
			component_hook<ComponentTypeList>::remove_all(*this, db_);
		}

		//----------------------------------------------------------------------------------------------
		template<typename ComponentType>
		inline component<ComponentType> getSafe() const
		{
			DJAH_ASSERT( isUsing<ComponentType>() );
			return db_.get<ComponentType>(component_usage<ComponentType>::cid_);
		}

		//----------------------------------------------------------------------------------------------
		template<typename ComponentType>
		inline ComponentType& get() const
		{
			return getSafe<ComponentType>().data();
		}

	private:
		static component_database<ComponentTypeList> db_;
		std::string name_;
	};
	//--------------------------------------------------------------------------------------------------
	template<typename ComponentTypeList>
	component_database<ComponentTypeList> game_object<ComponentTypeList>::db_;
	//==================================================================================================

} /*gameplay*/ } /*djah*/

#endif /* DJAH_GAMEPLAY_GAME_OBJECT_HPP */