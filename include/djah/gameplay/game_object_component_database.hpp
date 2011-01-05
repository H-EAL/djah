#ifndef DJAH_GAMEPLAY_GAME_OBJECT_COMPONENT_DATABASE_HPP
#define DJAH_GAMEPLAY_GAME_OBJECT_COMPONENT_DATABASE_HPP

#include "../utils/hierarchy_generation.hpp"
#include "game_object_component.hpp"

namespace djah { namespace gameplay {

	//----------------------------------------------------------------------------------------------
	class game_object_component_database;

	template<typename T>
	class game_object_component_id
	{
	public:
		typedef typename T::InnerType InnerType;
		typedef typename game_object_component<T>::Component Component;

		game_object_component_id(game_object_component_database &db, size_t offset)
			: db_(db), offset_(offset) {}

		game_object_component_id(const game_object_component_id &gocid)
			: db_(gocid.db_), offset_(gocid.offset_) {}

		operator InnerType() { return get(); }

		size_t offset() const { return offset_; }

		game_object_component_id<T>& operator =(const InnerType data) { set(data); return *this; }

	private:
		InnerType&	get()						{ return db_.get<T>(offset_).get(); }
		void		set(const InnerType data)	{ db_.get<T>(offset_).get() = data; }

		game_object_component_database &db_;
		size_t offset_;
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	struct component_holder
	{
		typedef std::vector< game_object_component<T> > component_list_t;
		component_list_t component_list_;
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	typedef TYPELIST_4(Position_Component, Velocity_Component, Orientation_Component, Speed_Component) ComponentTypes;
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class game_object_component_database
		: utils::gen_scatter_hierarchy<ComponentTypes, component_holder>
	{
	public:

		template<typename T>
		T add();

		template<typename T>
		void remove(const game_object_component_id<typename T::Component> &gocid);

		template<typename T>
		game_object_component<T>& get(size_t offset);

		template<typename T>
		const game_object_component<T>& get(size_t offset) const;	
	};
	//----------------------------------------------------------------------------------------------

} /*gameplay*/ } /*djah*/

#include "game_object_component_database.inl"

#endif /* DJAH_GAMEPLAY_GAME_OBJECT_COMPONENT_DATABASE_HPP */