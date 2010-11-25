#ifndef DJAH_RESOURCES_RESOURCE_BASE_HPP
#define DJAH_RESOURCES_RESOURCE_BASE_HPP

#include <string>
#include <boost/shared_ptr.hpp>

namespace djah { namespace resources {

	class resource_base
	{
	public:
		resource_base();
		const std::string& name() const;
		void			   setName(const std::string &name);

	protected:
		virtual ~resource_base();

	private:
		std::string name_;
	};

	typedef boost::shared_ptr<resource_base> resource_ptr;

} /*resources*/ } /*djah*/

#endif /* DJAH_RESOURCES_RESOURCE_BASE_HPP */