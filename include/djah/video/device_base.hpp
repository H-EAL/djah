#ifndef DJAH_VIDEO_DEVICE_BASE_HPP
#define DJAH_VIDEO_DEVICE_BASE_HPP

#include <string>
#include "video_config.hpp"
#include "driver_base.hpp"

namespace djah { namespace video {
	
	//----------------------------------------------------------------------------------------------
	class device_base
	{
	public:

		device_base();
		virtual ~device_base();

		static device_base* get_current();
		
		const video_config& videoConfig() const;

		void setVideoDriver(driver_ptr driver);
		driver_ptr videoDriver() const;

		virtual void create(const video_config &cfg);
		virtual void destroy();
		virtual bool run();
		virtual void shutDown();
		virtual void resize(int width, int height);

		virtual void show() = 0;
		virtual bool isWindowActive() = 0;
		virtual bool hasWindowFocus() = 0;

		virtual void setWindowTitle(const std::string &title) = 0;

		virtual void swapBuffers() = 0;

	protected:

		virtual void createImpl() = 0;
		virtual void destroyImpl() = 0;
		virtual bool runImpl() = 0;

		bool hasToQuit_;

		// Video driver used
		driver_ptr driver_;

		// Current config
		video_config video_config_;

		// Last created device
		static device_base *s_device_inst_;
	};
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	// Pointer to a device : should be reference counted
	typedef device_base* device_ptr;
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	device_ptr create_device(const video_config &cfg);
	//----------------------------------------------------------------------------------------------
	device_ptr create_device(int width, int height,
							 int colorBits = 32, int depthBits = 24, int stencilBits = 0,
							 bool fullscreen = false, bool vsync = true);
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	// Prototype of the actual instance creator (platform specific)
	//----------------------------------------------------------------------------------------------
	device_ptr new_platform_specific_device();
	//----------------------------------------------------------------------------------------------

} /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_DEVICE_BASE_HPP */