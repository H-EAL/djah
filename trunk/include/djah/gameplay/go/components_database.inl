namespace djah { namespace gameplay { namespace go {

	//----------------------------------------------------------------------------------------------
	template<typename T>
	T components_database::add()
	{
		getArray<T>().push_back(component<T::Component>());
		return T(*this, getArray<T>().size() - 1);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	void components_database::remove(const component_id<typename T::Component>& comp_id)
	{
		getArray<T>().erase(getArray<T>().begin() + comp_id.index());
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	component<T>& components_database::get(size_t index)
	{
		return getArray< component<T> >()[index];
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	typename component_holder<typename T::Component>::component_list_t& components_database::getArray()
	{
		return component_holder<T::Component>::component_list_;
	}
	//----------------------------------------------------------------------------------------------

} /*go*/ } /*gameplay*/ } /*djah*/