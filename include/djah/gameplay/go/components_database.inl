namespace djah { namespace gameplay { namespace go {

	//----------------------------------------------------------------------------------------------
	template<typename T>
	T components_database::add()
	{
		typedef typename T::Component					Component;
		typedef typename component_holder<Component>	ComponentHolder;
		typedef typename T								Id;

		ComponentHolder::component_list_.push_back(component<Component>());
		return Id(*this, ComponentHolder::component_list_.size() - 1);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	void components_database::remove(const component_id<typename T::Component>& comp_id)
	{
		typedef component_holder<typename T::Component> ComponentHolder;
		ComponentHolder::component_list_.erase(ComponentHolder::component_list_.begin() + comp_id.offset());
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	component<T>& components_database::get(size_t offset)
	{
		typedef component_holder<T> ComponentHolder;
		return ComponentHolder::component_list_[offset];
	}
	//----------------------------------------------------------------------------------------------

} /*go*/ } /*gameplay*/ } /*djah*/