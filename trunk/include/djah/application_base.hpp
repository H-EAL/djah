#ifndef DJAH_APPLICATION_BASE_HPP
#define DJAH_APPLICATION_BASE_HPP

#include "system/video_config.hpp"

#define DJAH_ENTRY_POINT(App) djah::application_base* djah_new_app() { return new App; }

namespace djah {

	namespace system {

		class video_config;
		class device_base;
		class driver_base;

	} /*system*/

	class application_base
	{
	public:

		explicit application_base(const system::video_config &config);
		virtual ~application_base();

		void init();
		int exec(int argc, char *argv[]);
		void exit();

	protected:

		virtual void initImpl() = 0;
		virtual void runImpl() = 0;
		virtual void exitImpl() = 0;

		system::video_config video_config_;
		system::device_base *device_;
		system::driver_base *driver_;
	};

} /*djah*/

extern djah::application_base* djah_new_app();

#endif /* DJAH_APPLICATION_BASE_HPP */