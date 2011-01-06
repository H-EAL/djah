#ifndef DJAH_GAMEPLAY_GO_COMPONENTS_DATABASE_HPP
#define DJAH_GAMEPLAY_GO_COMPONENTS_DATABASE_HPP

#include "../../utils/hierarchy_generation.hpp"
#include "component.hpp"

namespace djah { namespace gameplay { namespace go {

	//----------------------------------------------------------------------------------------------
	class components_database;
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	class component_id
	{
	public:
		typedef typename T::RealType	RealType;
		typedef typename T				Component;

		static u32 get_type() { return Component::name_; }

		component_id(components_database &db, size_t offset)
			: db_(db), offset_(offset) {}

		size_t offset() const { return offset_; }

		RealType& toRealType() { return get(); }
		operator RealType() { return get(); }
		component_id<T>& operator =(const RealType &data) { set(data); return *this; }

	private:
		RealType&	get()						{ return db_.get<T>(offset_).get(); }
		void		set(const RealType &data)	{ get() = data; }

		components_database &db_;
		size_t offset_;
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	struct component_holder
	{
		typedef std::vector< component<T> > component_list_t;
		component_list_t component_list_;
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	#include "components_typelist.inl"
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	class components_database
		: utils::gen_scatter_hierarchy<ComponentTypes, component_holder>
	{
	public:

		template<typename T>
		T add();

		template<typename T>
		void remove(const component_id<typename T::Component> &comp_id);

		template<typename T>
		component<T>& get(size_t offset);

		template<typename T>
		const component<T>& get(size_t offset) const;	
	};
	//----------------------------------------------------------------------------------------------

} /*go*/ } /*gameplay*/ } /*djah*/

#include "components_database.inl"

#endif /* DJAH_GAMEPLAY_GO_COMPONENTS_DATABASE_HPP */