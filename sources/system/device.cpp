#include "djah/system/device.hpp"
#include "djah/system/gl.hpp"
#include "djah/system/opengl_driver.hpp"
#include "djah/system/context.hpp"


namespace djah { namespace system {

	//----------------------------------------------------------------------------------------------
	device_ptr create_device(const video_config &cfg)
	{
		device_ptr dev	  = new device;
		driver_ptr driver = new opengl_driver;
		dev->setVideoDriver(driver);
		dev->create(cfg);
		driver->create();
		return dev;
	}
	//----------------------------------------------------------------------------------------------
	device_ptr create_device(int width, int height,
							 int colorBits, int depthBits, int stencilBits,
							 bool fullscreen, bool vsync)
	{
		return create_device( video_config(width, height, colorBits, depthBits, stencilBits, fullscreen, vsync) );
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	device_ptr device::sp_device_inst_ = 0;
	//----------------------------------------------------------------------------------------------

} /*system*/ } /*djah*/