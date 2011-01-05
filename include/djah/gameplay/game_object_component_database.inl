namespace djah { namespace gameplay {

	//----------------------------------------------------------------------------------------------
	template<typename T>
	T game_object_component_database::add()
	{
		typedef typename T::Component					Component;
		typedef typename component_holder<Component>	ComponentHolder;
		typedef typename T								Id;

		ComponentHolder::component_list_.push_back(game_object_component<Component>());
		return Id(*this, ComponentHolder::component_list_.size() - 1);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	void game_object_component_database::remove(const game_object_component_id<typename T::Component>& gocid)
	{
		typedef typename component_holder<typename T::Component> ComponentHolder;

		ComponentHolder::component_list_.erase(ComponentHolder::component_list_.begin() + gocid.offset());
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	game_object_component<T>& game_object_component_database::get(size_t offset)
	{
		typedef typename component_holder<T> ComponentHolder;
		return ComponentHolder::component_list_[offset];
	}
	//----------------------------------------------------------------------------------------------

} /*gameplay*/ } /*djah*/