#ifndef DJAH_GAMEPLAY_COMPONENT_PROCESS_HPP
#define DJAH_GAMEPLAY_COMPONENT_PROCESS_HPP

#include <set>
#include <vector>
#include <memory>
#include <type_traits>
#include "djah/gameplay/game_object.hpp"

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
		typename ComponentTypeList,
		typename MandatoryComponents,
		bool Ordered = false,
		typename Compare = std::less< game_object<ComponentTypeList>* >
	>
	class component_process
		: public interfaces::process
	{
	public:
		typedef game_object<ComponentTypeList> game_object_t;
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
		};

		typedef typename std::conditional<Ordered, ordered_container, unordered_container>::type go_container_t;
		go_container_t gameObjects_;
	};

} /*gameplay*/ } /*djah*/

#endif /* DJAH_GAMEPLAY_COMPONENT_PROCESS_HPP */