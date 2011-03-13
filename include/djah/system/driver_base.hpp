#ifndef DJAH_SYSTEM_DRIVER_BASE_HPP
#define DJAH_SYSTEM_DRIVER_BASE_HPP

#include "../math/matrix4.hpp"
#include "../geometry/rect.hpp"

namespace djah { namespace system {

	// Forward declaration
	class device_base;
	
	//----------------------------------------------------------------------------------------------
	class driver_base
	{
	public:

		driver_base(device_base *device);
		virtual ~driver_base();

		virtual void create() = 0;
		virtual void destroy() = 0;

		virtual void beginScene() = 0;
		virtual void endScene() = 0;

		void setViewport(const geometry::rect_i &viewport);
		const geometry::rect_i& getViewport() const;

	protected:

		virtual void updateViewport() = 0;

		device_base			*device_;

		geometry::rect_i	viewport_;
	};
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	// Pointer to a driver : should be reference counted
	//----------------------------------------------------------------------------------------------
	typedef driver_base* driver_ptr;
	//----------------------------------------------------------------------------------------------

} /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_DRIVER_BASE_HPP */
