#include <algorithm>
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
			width, height, colorBits, depthBits, stencilBits/*, fullscreen, title*/
		));
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	device* device::sp_device_inst_ = nullptr;
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	std::shared_ptr<driver> device::createDriver(const std::shared_ptr<driver_config> &pDriverConfig,
												 const std::shared_ptr<driver> &pSharedDriver)
	{
		auto pDriver = std::make_shared<driver>(this, pDriverConfig, pSharedDriver);
		pLinkedDrivers_.push_back(pDriver);
		return pDriver;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void device::removeDriver(const std::shared_ptr<driver> &pDriver)
	{
		auto it = std::find_if(pLinkedDrivers_.begin(), pLinkedDrivers_.end(), [&](const std::weak_ptr<driver> &wpDriver)
		{
			return !wpDriver.expired() && wpDriver.lock() == pDriver;
		});

		if( it != pLinkedDrivers_.end() )
		{
			pLinkedDrivers_.erase(it);
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool device::setResolution(bool fullScreen)
	{
		return setResolution(pConfig_->width, pConfig_->height, fullScreen);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool device::setResolution(int width, int height)
	{
		return setResolution(width, height, pConfig_->fullscreen);
	}
	//----------------------------------------------------------------------------------------------

} /*system*/ } /*djah*/