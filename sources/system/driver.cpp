#include "djah/system/driver.hpp"
#include "djah/system/device.hpp"
#include "djah/system/context.hpp"
#include "djah/system/video_config.hpp"
#include "djah/system/gl.hpp"
#include "djah/debug/assertion.hpp"
#include <iostream>


namespace djah { namespace system {

	//----------------------------------------------------------------------------------------------
	driver_sptr create_driver(const std::shared_ptr<device> &pDevice, const driver_config_sptr &pConfig)
	{
		check(pDevice);
		return std::make_shared<driver>(pDevice, pConfig);
	}
	//----------------------------------------------------------------------------------------------
	driver_sptr create_driver(const std::shared_ptr<device> &pDevice, eOpenglVersion version,
							  bool enableDebug, bool enableCompatibilityProfile)
	{
		return create_driver(pDevice, std::make_shared<driver_config>(
			version, enableDebug, enableCompatibilityProfile
		));
	}
	//----------------------------------------------------------------------------------------------


	//-------------------------------------------------------------------------------------------------
	driver::driver(const std::shared_ptr<device> &_pDevice, const driver_config_sptr &_pConfig)
		: pDevice_(_pDevice)
		, pConfig_(_pConfig)
	{
		static bool firstContextCreation = true;
		if( firstContextCreation )
		{
			driver_config_sptr pDefaultConfig = std::make_shared<driver_config>();
			gl_context defaultContext(pDevice_, pDefaultConfig);

			pContext_ = std::unique_ptr<gl_context>(new gl_context(pDevice_, pConfig_));
			check(pContext_->isValid());

			pContext_->makeCurrent();

			firstContextCreation = false;
		}
		else
		{
			pContext_ = std::unique_ptr<gl_context>(new gl_context(pDevice_, pConfig_));
			check(pContext_->isValid());

			pContext_->makeCurrent();
		}

		load_extensions();
		capabilities_.init();
	}
	//-------------------------------------------------------------------------------------------------


	//-------------------------------------------------------------------------------------------------
	driver::~driver()
	{
	}
	//-------------------------------------------------------------------------------------------------


	//-------------------------------------------------------------------------------------------------
	void driver::beginFrame()
	{
	}
	//-------------------------------------------------------------------------------------------------


	//-------------------------------------------------------------------------------------------------
	void driver::endFrame()
	{
		pContext_->swapBuffers();
	}
	//-------------------------------------------------------------------------------------------------


	//-------------------------------------------------------------------------------------------------
	void driver::setVSync(bool enabled)
	{
		pContext_->setVSync(enabled);
	}
	//-------------------------------------------------------------------------------------------------


	/*
	//-------------------------------------------------------------------------------------------------
	void driver::setViewport(const geometry::rect_i &viewport)
	{
		viewport_ = viewport;
		const math::vector2i &topLeft = viewport_.topLeft();
		glViewport(topLeft.x, topLeft.y, viewport_.width(), viewport_.height());
	}
	//-------------------------------------------------------------------------------------------------


	//-------------------------------------------------------------------------------------------------
	const geometry::rect_i& driver::getViewport() const
	{
		return viewport_;
	}
	//-------------------------------------------------------------------------------------------------
	*/

} /*system*/ } /*djah*/
