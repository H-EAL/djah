#include "djah/system/driver_base.hpp"
#include "djah/system/device.hpp"

namespace djah { namespace system {

	//-------------------------------------------------------------------------------------------------
	driver_base::driver_base()
		: glContext_( gl_format(OpenGL_Version_4_3, true) )
	{
	}
	//-------------------------------------------------------------------------------------------------


	//-------------------------------------------------------------------------------------------------
	driver_base::~driver_base()
	{
	}
	//-------------------------------------------------------------------------------------------------


	//-------------------------------------------------------------------------------------------------
	void driver_base::setViewport(const geometry::rect_i &viewport)
	{
		viewport_ = viewport;
		updateViewport();
	}
	//-------------------------------------------------------------------------------------------------


	//-------------------------------------------------------------------------------------------------
	const geometry::rect_i& driver_base::getViewport() const
	{
		return viewport_;
	}
	//-------------------------------------------------------------------------------------------------

} /*system*/ } /*djah*/
