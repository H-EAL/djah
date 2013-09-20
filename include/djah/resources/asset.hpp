#ifndef DJAH_RESOURCES_ASSET_HPP
#define DJAH_RESOURCES_ASSET_HPP

#include <string>
#include <memory>
#include "djah/types.hpp"

namespace djah { namespace resources {

	class asset
	{
	public:
		asset();
		const std::string& name() const;
		void			   setName(const std::string &name);
		virtual u32		   size() const { return 0; }

	protected:
		virtual ~asset();

	private:
		std::string name_;
	};

	typedef std::shared_ptr<asset>	asset_sptr;
	typedef std::weak_ptr<asset>	asset_wptr;

} /*resources*/ } /*djah*/

#endif /* DJAH_RESOURCES_ASSET_HPP */