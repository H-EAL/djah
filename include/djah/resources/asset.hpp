#ifndef DJAH_RESOURCES_ASSET_HPP
#define DJAH_RESOURCES_ASSET_HPP

#include <string>
#include <memory>
#include "djah/types.hpp"

namespace djah { namespace resources {

	class asset
	{
	public:
		asset(const std::string &_name = "", u64 _timestamp = 0);
		const std::string& name() const;
		void			   setName(const std::string &name);
		virtual u32		   size() const { return 0; }
		u64				   timestamp() const { return timestamp_; }
		void			   setTimestamp(u64 _timestamp) { timestamp_ = _timestamp; }

		asset& operator =(const asset &rhs);

	protected:
		virtual ~asset();

	protected:
		std::string name_;
		u64			timestamp_;
	};

	typedef std::shared_ptr<asset>	asset_sptr;
	typedef std::weak_ptr<asset>	asset_wptr;

} /*resources*/ } /*djah*/

#endif /* DJAH_RESOURCES_ASSET_HPP */