#ifndef DJAH_SYSTEM_DRIVER_HPP
#define DJAH_SYSTEM_DRIVER_HPP

#include <memory>
#include "djah/system/video_config.hpp"
#include "djah/opengl/capabilities.hpp"

namespace djah { namespace system {

	//----------------------------------------------------------------------------------------------
	// Forward declarations
	//----------------------------------------------------------------------------------------------
	class driver;
	class device;
	class gl_context;
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	// Useful type definitions
	//----------------------------------------------------------------------------------------------
	typedef std::shared_ptr<driver> driver_sptr;
	typedef std::weak_ptr<driver>	driver_wptr;
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	// Video driver
	//----------------------------------------------------------------------------------------------
	class driver
	{
	public:
		driver(device *_pDevice, const driver_config_sptr &_pConfig, const driver_sptr &pSharedDriver);
		~driver();

		void beginFrame();
		void endFrame();

		void setVSync(bool enabled);
		void enableVSync()				{ setVSync(true);	}
		void disableVSync()				{ setVSync(false);	}

		const driver_config_sptr&	config() const	{ return pConfig_;	}
		driver_config_sptr&			config()		{ return pConfig_;	}

		opengl::capabilities& caps() { return capabilities_; }

	private:
		// No copy
		driver(const driver &);
		driver& operator =(const driver &);

	private:
		driver_config_sptr				pConfig_;
		std::unique_ptr<gl_context>		pContext_;
		opengl::capabilities			capabilities_;
	};
	//----------------------------------------------------------------------------------------------

} /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_DRIVER_HPP */
