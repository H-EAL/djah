#ifndef DJAH_GAMEPLAY_SERVICES_GAME_SERVICE_HPP
#define DJAH_GAMEPLAY_SERVICES_GAME_SERVICE_HPP

#include <set>
#include <vector>
#include <memory>
#include <type_traits>
#include "djah/gameplay/game_object.hpp"

namespace djah { namespace gameplay { namespace services {

	namespace interfaces {
		class service
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
	class game_service
		: public interfaces::service
	{
	public:
		typedef game_object<ComponentTypeList> game_object_t;
		typedef game_object_t *game_object_ptr;

	public:
		virtual ~game_service()
		{
		}

		virtual bool add(game_object_t *pGameObject)
		{
			check(pGameObject);
			const bool canAddGO = pGameObject->isUsingList<MandatoryComponents>();
			
			if(canAddGO)
			{
				game_objects_.add(pGameObject);
			}

			return canAddGO;
		}

		virtual void execute(float dt)
		{
			auto itEnd = game_objects_.impl_.end();
			for(auto it = game_objects_.impl_.begin(); it != itEnd; ++it)
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
		go_container_t game_objects_;
	};

} /*services*/ } /*gameplay*/ } /*djah*/

#endif /* DJAH_GAMEPLAY_SERVICES_GAME_SERVICE_HPP */