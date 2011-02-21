#ifndef DJAH_TESTS_APPLICATION_BASE_HPP
#define DJAH_TESTS_APPLICATION_BASE_HPP

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

		void exec();

	protected:

		virtual void init();
		virtual void run();
		virtual void exit();

		video::device_base *device_;
		video::driver_base *driver_;
	};

} /*tests*/ } /*djah*/

#endif /* DJAH_TESTS_APPLICATION_BASE_HPP */