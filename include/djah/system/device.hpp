#ifndef DJAH_SYSTEM_DEVICE_HPP
#define DJAH_SYSTEM_DEVICE_HPP

#include <vector>
#include <string>
#include <memory>

namespace djah { namespace system {

	//----------------------------------------------------------------------------------------------
	// Forward declarations
	//----------------------------------------------------------------------------------------------
	class device;
	class device_config;
	class driver;
	class driver_config;
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

		// Create a driver linked to this device
		std::shared_ptr<driver> createDriver(const std::shared_ptr<driver_config> &pDriverConfig,
											 const std::shared_ptr<driver> &pSharedDriver = std::shared_ptr<driver>());
		void removeDriver(const std::shared_ptr<driver> &pDriver);

		// Pointer to current device
		static device* get_current()	{ return sp_device_inst_;	}

		// Casted platform specific device handle
		template<typename T>
		T handle()						{ return static_cast<T>(internalHandle());	}
		
		const std::shared_ptr<device_config>&	config() const	{ return pConfig_;	}
		std::shared_ptr<device_config>&			config()		{ return pConfig_;	}

		bool run();
		void shutDown();

		// Change resolution on the fly
		bool setResolution(bool fullScreen);
		bool setResolution(int width, int height);
		bool setResolution(int width, int height, bool fullScreen);

		void show();
		bool isActive() const;
		bool hasFocus() const;
		void setTitle(const std::string &title);

	private:
		// No copy
		device(const device &);
		device& operator =(const device &);

		// Platform specific device handle
		void* internalHandle() const;

	private:
		// List of drivers linked to this device
		std::vector<std::weak_ptr<driver>> pLinkedDrivers_;

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