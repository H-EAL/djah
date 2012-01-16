#ifndef DJAH_SYSTEM_DEVICE_HPP
#define DJAH_SYSTEM_DEVICE_HPP

#include <string>
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

		static device* get_current()				{ return sp_device_inst_;	}
		device_impl* impl()					const	{ return p_impl_;			}
		
		const video_config& videoConfig()	const	{ return video_config_;		}

		void setVideoDriver(driver_ptr driver)		{ p_driver_ = driver;			} 
		driver_ptr videoDriver()			const	{ return p_driver_;			}

		void create(const video_config &cfg);
		void destroy();
		bool run();
		void shutDown();
		void resize(int width, int height);

		void show();
		bool isWindowActive();
		bool hasWindowFocus();

		void setWindowTitle(const std::string &title);

		void swapBuffers();

	private:

		bool hasToQuit_;

		// Video driver used
		driver_ptr p_driver_;

		// Current config
		video_config video_config_;

		// Private implementation, platform dependant
		device_impl	*p_impl_;

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