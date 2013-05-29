#ifndef DJAH_SYSTEM_DEVICE_HPP
#define DJAH_SYSTEM_DEVICE_HPP

#include <string>
#include <memory>
#include "djah/math/vector2.hpp"

namespace djah { namespace system {

	//----------------------------------------------------------------------------------------------
	// Forward declarations
	//----------------------------------------------------------------------------------------------
	class device;
	class driver;
	class device_config;
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	// Useful type definitions
	//----------------------------------------------------------------------------------------------
	typedef std::shared_ptr<device> device_sptr;
	typedef std::weak_ptr<device>   device_wptr;
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	// Device class
	//----------------------------------------------------------------------------------------------
	class device
	{
	public:
		device(const std::shared_ptr<device_config> &_config);
		~device();

		static device* get_current()	{ return sp_device_inst_;	}

		template<typename T>
		T handle()						{ return static_cast<T>(internalHandle()); }
		
		const std::shared_ptr<device_config>&	config() const	{ return pConfig_;	}
		std::shared_ptr<device_config>&			config()		{ return pConfig_;	}

		bool run();
		void shutDown();

		void resize(int width, int height);
		void toggleFullScreen(bool fullScreen);
		void show();
		bool isActive() const;
		bool hasFocus() const;
		void setTitle(const std::string &title);

		void setActiveDriver(const std::weak_ptr<driver> &activeDriver);

		math::vector2i clientMousePosition(const math::vector2i &screenMousePos) const;
		math::vector2i screenMousePosition(const math::vector2i &clientMousePos) const;

	private:
		// no copy
		device(const device &);
		device& operator =(const device &);

		void* internalHandle() const;

	private:
		// Private implementation, platform dependent
		std::unique_ptr<class device_impl> pImpl_;

		// Current config
		std::shared_ptr<device_config> pConfig_;

		// Set to true when the application is shut down
		bool hasToQuit_;

		// Last created device
		static device *sp_device_inst_;
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	// Helper function to create a device
	//----------------------------------------------------------------------------------------------
	device_sptr create_device(const std::shared_ptr<device_config> &_config);
	//----------------------------------------------------------------------------------------------
	device_sptr create_device(int width, int height, int colorBits = 32, int depthBits = 24,
							  int stencilBits = 0, bool fullscreen = false,
							  const std::string &title = "Djah's Heavenly Window");
	//----------------------------------------------------------------------------------------------

} /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_DEVICE_HPP */