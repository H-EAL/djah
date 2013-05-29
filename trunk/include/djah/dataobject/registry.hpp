#ifndef DJAH_DATA_OBJECT_REGISTRY_HPP
#define DJAH_DATA_OBJECT_REGISTRY_HPP

#include <map>
#include <memory>
#include "../filesystem/browser.hpp"
#include "../debug/log.hpp"
#include "data_object.hpp"

namespace djah { namespace dataobject {

	//----------------------------------------------------------------------------------------------
	template<typename AttributeTypes, template<typename> class Serializer>
	class registry
	{
	private:
		typedef typename data_object<AttributeTypes>		data_object_t;

	public:
		typedef std::shared_ptr<data_object_t>				data_object_ptr;

	public:
		bool			has(const std::string &dataObjectName) const;
		data_object_ptr get(const std::string &dataObjectName);
		void			reload(const std::string &dataObjectName);
		void			save(const std::string &dataObjectName);

	private:
		data_object_ptr load(const std::string &dataObjectName) const;
		void			add(data_object_ptr dataObject);

	private:
		typedef std::map<std::string, data_object_ptr>		data_object_list_t;

		data_object_list_t dataObjects_;
	};
	//----------------------------------------------------------------------------------------------

} /*dataobject*/ } /*djah*/

#include "registry.inl"

#endif /* DJAH_DATA_OBJECT_REGISTRY_HPP */