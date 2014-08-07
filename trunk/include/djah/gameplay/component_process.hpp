#ifndef DJAH_GAMEPLAY_COMPONENT_PROCESS_HPP
#define DJAH_GAMEPLAY_COMPONENT_PROCESS_HPP

#include <set>
#include <vector>
#include <memory>
#include <type_traits>
#include "djah/gameplay/entity.hpp"

namespace djah { namespace gameplay {

	namespace interfaces {
		class process
		{
		public:
			virtual void execute(float dt) = 0;
		};
	};


	template
	<
		typename ComponentsTypeList,
		typename MandatoryComponents,
		bool Ordered = false,
		typename Compare = std::less< entity_t<ComponentsTypeList>* >
	>
	class component_process
		: public interfaces::process
	{
	public:
		typedef entity_t<ComponentsTypeList> game_object_t;
		typedef game_object_t *game_object_ptr;

	public:
		virtual ~component_process()
		{
		}

		virtual bool add(game_object_t *pGameObject)
		{
			check(pGameObject);
			const bool canAddGO = pGameObject->isUsingList<MandatoryComponents>();
			
			if(canAddGO)
			{
				gameObjects_.add(pGameObject);
			}

			return canAddGO;
		}

		virtual void remove(game_object_t *pEntity)
		{
			gameObjects_.remove(pEntity);
		}

		virtual void execute(float dt)
		{
			auto itEnd = gameObjects_.impl_.end();
			for(auto it = gameObjects_.impl_.begin(); it != itEnd; ++it)
			{
				executeFor(*(*it), dt);
			}
		}

	protected:
		virtual void executeFor(game_object_t &gameObject, float dt) = 0;

	protected:
		struct ordered_container
		{
			typedef std::set<game_object_ptr, Compare> go_list_t;
			go_list_t impl_;

			size_t size() const
			{
				return impl_.size();
			}

			void add(game_object_ptr pGo)
			{
				impl_.insert(pGo);
			}

			void remove(game_object_ptr pEntity)
			{
				impl_.erase(pEntity);
			}
		};

		struct unordered_container
		{
			typedef std::vector<game_object_ptr> go_list_t;
			go_list_t impl_;

			size_t size() const
			{
				return impl_.size();
			}

			void add(game_object_ptr pGo)
			{
				impl_.push_back(pGo);
			}

			void remove(game_object_ptr pEntity)
			{
				auto it = std::find(impl_.begin(), impl_.end(), pEntity);
				if( it != impl_.end() )
				{
					impl_.erase(it);
				}
			}
		};

		typedef typename std::conditional<Ordered, ordered_container, unordered_container>::type go_container_t;
		go_container_t gameObjects_;
	};

} /*gameplay*/ } /*djah*/

#endif /* DJAH_GAMEPLAY_COMPONENT_PROCESS_HPP */