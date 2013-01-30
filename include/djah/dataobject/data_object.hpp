#ifndef DATA_OBJECT_HPP
#define DATA_OBJECT_HPP

#include <map>
#include <string>
#include "djah/debug/assertion.hpp"
#include "djah/core/typelist.hpp"
#include "djah/core/hierarchy_generation.hpp"
#include "djah/dataobject/attribute.hpp"

namespace djah { namespace dataobject {

	//----------------------------------------------------------------------------------------------
	// An attribute hook enables the iteration on all the attributes of all types
	template<typename AttributeTypes>
	struct attribute_hook;
	//----------------------------------------------------------------------------------------------
	template<>
	struct attribute_hook<utils::nulltype>
	{
		template<typename DO>
		static bool has(const DO&, const std::string&) { return false; }
	};
	//----------------------------------------------------------------------------------------------
	template<typename HeadAttribute, typename TailList>
	struct attribute_hook< utils::typelist<HeadAttribute, TailList> >
	{
		template<typename DO>
		static bool has(const DO &dobj, const std::string &attributeName)
		{
			return dobj.has<HeadAttribute>(attributeName)
				|| attribute_hook<TailList>::has(dobj, attributeName);
		}
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	// An attribute holder holds all attributes of one type for one data object
	template<typename T>
	struct attribute_holder
	{
		typedef typename attribute<T> attribute_ptr;
		typedef std::map<std::string, attribute_ptr> attribute_map_t;

		attribute_map_t attributes_;
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	// A data object is a collection of attributes of different types
	template<typename AttributeTypes>
	class data_object
		: public utils::gen_scatter_hierarchy<AttributeTypes, attribute_holder>
	{
	public:
		//------------------------------------------------------------------------------------------
		data_object(const std::string &name)
			: name_(name)
		{
		}
		//------------------------------------------------------------------------------------------

		//------------------------------------------------------------------------------------------
		const std::string& name() const { return name_; }
		//------------------------------------------------------------------------------------------

		//------------------------------------------------------------------------------------------
		template<typename T>
		T get(const std::string &attributeName, const T &default_value = T()) const
		{
			T result = default_value;

			auto it = attribute_holder<T>::attributes_.find(attributeName);
			if( it != attribute_holder<T>::attributes_.end() )
				result = it->second.value;

			return result;
		}
		//------------------------------------------------------------------------------------------

		//------------------------------------------------------------------------------------------
		template<typename T>
		bool has(const std::string &attributeName) const
		{
			auto it = attribute_holder<T>::attributes_.find(attributeName);
			return (it != attribute_holder<T>::attributes_.end());
		}
		//------------------------------------------------------------------------------------------

		//------------------------------------------------------------------------------------------
		bool has(const std::string &attributeName) const
		{
			return attribute_hook<AttributeTypes>::has(*this, attributeName);
		}
		//------------------------------------------------------------------------------------------

		//------------------------------------------------------------------------------------------
		template<typename T>
		void add(const attribute<T> &attr)
		{
			DJAH_ASSERT_MSG( !has<T>(attr.name), "data_object[%s]::add(%s), trying to add an already existing attribute", name_.c_str(), attr.name.c_str() );
			attribute_holder<T>::attributes_[attr.name] = attr;
		}
		//----------------------------------------------------------------------------------------------

	private:
		std::string name_;
	};
	//----------------------------------------------------------------------------------------------

} /*dataobject*/ } /*djah*/

#endif /* DATA_OBJECT_HPP */