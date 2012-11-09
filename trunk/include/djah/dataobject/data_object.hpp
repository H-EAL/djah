#ifndef DATA_OBJECT_HPP
#define DATA_OBJECT_HPP

#include <map>
#include <string>
#include "../debug/assertion.hpp"
#include "../core/typelist.hpp"
#include "../core/hierarchy_generation.hpp"
#include "attribute.hpp"

namespace djah { namespace dataobject {

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
		T get(const std::string &attribute_name, const T &default_value = T()) const
		{
			T result = default_value;

			auto it = attribute_holder<T>::attributes_.find(attribute_name);
			if( it != attribute_holder<T>::attributes_.end() )
				result = it->second.value;

			return result;
		}
		//------------------------------------------------------------------------------------------

		//------------------------------------------------------------------------------------------
		template<typename T>
		bool has(const std::string &attribute_name) const
		{
			auto it = attribute_holder<T>::attributes_.find(attribute_name);
			return (it != attribute_holder<T>::attributes_.end());
		}
		//------------------------------------------------------------------------------------------

		//------------------------------------------------------------------------------------------
		template<typename T>
		void add(const attribute<T> &attr)
		{
			DJAH_ASSERT_MSG( !has<T>(attr.name), "data_object::add(), trying to add an already existing attribute" );
			attribute_holder<T>::attributes_[attr.name] = attr;
		}
		//----------------------------------------------------------------------------------------------

	private:
		std::string name_;
	};
	//----------------------------------------------------------------------------------------------

} /*dataobject*/ } /*djah*/

#endif /* DATA_OBJECT_HPP */