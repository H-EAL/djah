#ifndef DJAH_TESTS_APPLICATION_BASE_HPP
#define DJAH_TESTS_APPLICATION_BASE_HPP

#include <djah/video/video_config.hpp>

namespace djah { namespace video {

		class video_config;
		class device_base;
		class driver_base;

} /*video*/ } /*djah*/

namespace djah { namespace tests {

	class application_base
	{
	public:

		explicit application_base(const video::video_config &config);
		virtual ~application_base();

		void init();
		int exec(int argc, char *argv[]);
		void exit();

	protected:

		virtual void initImpl() = 0;
		virtual void runImpl() = 0;
		virtual void exitImpl() = 0;

		video::video_config video_config_;
		video::device_base *device_;
		video::driver_base *driver_;
	};

} /*tests*/ } /*djah*/

#endif /* DJAH_TESTS_APPLICATION_BASE_HPP */