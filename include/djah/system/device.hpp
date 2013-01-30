#ifndef DJAH_SYSTEM_DEVICE_HPP
#define DJAH_SYSTEM_DEVICE_HPP

#include <string>
#include <memory>
#include "../platform.hpp"
#include "video_config.hpp"
#include "driver_base.hpp"

namespace djah { namespace system {

	// Forward declaration
	class device_impl;
	
	//----------------------------------------------------------------------------------------------
	class device
	{
	public:
		device();
		~device();

		void deleteTempContext();

		static device* get_current()				{ return sp_device_inst_;	}

		void* windowHandle() const;
		
		const video_config& videoConfig()	const	{ return videoConfig_;		}
		video_config&		videoConfig()			{ return videoConfig_;		}

		void setVideoDriver(driver_ptr driver)		{ pDriver_ = driver;		} 
		driver_ptr videoDriver()			const	{ return pDriver_;			}

		void create(const video_config &cfg);
		void destroy();
		bool run();
		void shutDown();
		void resize(int width, int height);

		void show();
		bool isWindowActive() const;
		bool hasWindowFocus() const;

		void setWindowTitle(const std::string &title);

		void setVSync(bool enabled);
		void enableVSync();
		void disableVSync();

		void swapBuffers();

		math::vector2i clientMousePosition(const math::vector2i &screenMousePos) const;
		math::vector2i screenMousePosition(const math::vector2i &clientMousePos) const;

	private:
		// Video driver used
		driver_ptr pDriver_;

		// Current config
		video_config videoConfig_;

		// Private implementation, platform dependent
		std::unique_ptr<device_impl> pImpl_;

		// Set to true when the application is shut down
		bool hasToQuit_;

		// Last created device
		static device *sp_device_inst_;
	};
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	// Pointer to a device : should be reference counted
	typedef device* device_ptr;
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	device_ptr create_device(const video_config &cfg);
	//----------------------------------------------------------------------------------------------
	device_ptr create_device(int width, int height,
							 int colorBits = 32, int depthBits = 24, int stencilBits = 0,
							 bool fullscreen = false, bool vsync = true);
	//----------------------------------------------------------------------------------------------

} /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_DEVICE_HPP */