#ifndef DJAH_CORE_EVENTS_HPP
#define DJAH_CORE_EVENTS_HPP

#include <queue>
#include <functional>
#include "djah/core/typelist.hpp"
#include "djah/core/hierarchy_generation.hpp"

namespace djah {

	template<typename EventsTypes>
	struct events_visitor;

	template<>
	struct events_visitor<utils::nulltype>
	{
		template<typename EP>
		static void process(EP &eventsPipe) {}
	};

	template<typename H, typename T>
	struct events_visitor<utils::typelist<H,T>>
	{
		template<typename EP>
		static void process(EP &eventPipes)
		{
			std::queue<H> &eventsList = eventPipes.events<H>();

			while( !eventsList.empty() )
			{
				eventPipes.broadcast(eventsList.front());
				eventsList.pop();
			}

			events_visitor<T>::process(eventPipes);
		}
	};

	template<typename EventType>
	struct event_holder
	{
		typedef std::function<void(const EventType&)> callback_t;

		std::vector<callback_t> callbacks_;
		std::queue<EventType>   events_;
	};

	template<typename EventsList>
	struct event_pipes
		: public utils::gen_scatter_hierarchy<EventsList, event_holder>
	{
		template<typename EventType>
		void subscribe(const typename event_holder<EventType>::callback_t &callback)
		{
			event_holder<EventType>::callbacks_.push_back(callback);
		}

		template<typename EventType>
		void unsubscribe(const typename event_holder<EventType>::callback_t &callback)
		{
			auto it = std::find(event_holder<EventType>::callbacks_.begin(), event_holder<EventType>::callbacks_.end(), callback);

			if( it != event_holder<EventType>::callbacks_.end() )
			{
				event_holder<EventType>::callbacks_.erase(it);
			}
		}

		template<typename EventType>
		void push(const EventType &event)
		{
			event_holder<EventType>::events_.push(event);
		}

		template<typename EventType>
		std::queue<EventType>& events()
		{
			return event_holder<EventType>::events_;
		}

		void process()
		{
			events_visitor<EventsList>::process(*this);
		}

		template<typename EventType>
		void broadcast(const EventType &event)
		{
			std::for_each(event_holder<EventType>::callbacks_.begin(), event_holder<EventType>::callbacks_.end(),
				[&event](event_holder<EventType>::callback_t &handler)
			{
				handler(event);
			});
		}
	};

} /*djah*/

#endif /* DJAH_CORE_EVENTS_HPP */