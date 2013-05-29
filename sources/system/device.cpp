#include "djah/system/gl.hpp"
#include "djah/system/device.hpp"
#include "djah/system/driver.hpp"
#include "djah/system/video_config.hpp"


namespace djah { namespace system {

	//----------------------------------------------------------------------------------------------
	device_sptr create_device(const std::shared_ptr<device_config> &_pConfig)
	{
		return std::make_shared<device>(_pConfig);
	}
	//----------------------------------------------------------------------------------------------
	device_sptr create_device(int width, int height, int colorBits, int depthBits, int stencilBits,
							  bool fullscreen, const std::string &title)
	{
		return create_device(std::make_shared<device_config>(
			width, height, colorBits, depthBits, stencilBits, fullscreen, title
		));
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	device* device::sp_device_inst_ = nullptr;
	//----------------------------------------------------------------------------------------------

} /*system*/ } /*djah*/