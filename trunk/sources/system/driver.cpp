#include "djah/system/driver.hpp"
#include "djah/system/device.hpp"
#include "djah/system/context.hpp"
#include "djah/system/video_config.hpp"
#include "djah/system/gl.hpp"
#include "djah/system/system_logger.hpp"
#include "djah/debug/assertion.hpp"


namespace djah { namespace system {

	//-------------------------------------------------------------------------------------------------
	driver::driver(device *_pDevice, const driver_config_sptr &_pConfig, const driver_sptr &pSharedDriver)
		: pConfig_(_pConfig)
	{
		DJAH_SYSTEM_NOTIFICATION()
			<< "Creating OpenGL context "
			<< _pConfig->majorVersion << "." << _pConfig->minorVersion
			<< "..." << DJAH_END_LOG();

		gl_context *pSharedContext = nullptr;
		if( pSharedDriver && pSharedDriver->pContext_ && pSharedDriver->pContext_->isValid() )
		{
			pSharedContext = pSharedDriver->pContext_.get();
		}

		pContext_ = std::unique_ptr<gl_context>(new gl_context(_pDevice, pConfig_, pSharedContext));
		check(pContext_->isValid());
		pContext_->makeCurrent();

		setVSync(pConfig_->vsync);

		load_extensions();
		capabilities_.init();

		DJAH_SYSTEM_NOTIFICATION()
			<< "OpenGL context created (version "
			<< capabilities_.value_of<std::string>(GL_VERSION)
			<< ")" << DJAH_END_LOG();
	}
	//-------------------------------------------------------------------------------------------------


	//-------------------------------------------------------------------------------------------------
	driver::~driver()
	{
		DJAH_SYSTEM_NOTIFICATION() << "Destroying driver..." << DJAH_END_LOG();
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

} /*system*/ } /*djah*/
