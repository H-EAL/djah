#include "application_base.hpp"

#include <djah/video/device_base.hpp>
#include <djah/video/driver_base.hpp>

namespace djah { namespace tests {

	//----------------------------------------------------------------------------------------------
	application_base::application_base(const video::video_config &config)
		: video_config_(config)
		, device_(0)
		, driver_(0)
	{
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	application_base::~application_base()
	{
		delete device_;
	}
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	int application_base::exec(int argc, char *argv[])
	{
		if( device_ && driver_ )
		{
			while( device_->run() )
				runImpl();

			return EXIT_SUCCESS;
		}

		return EXIT_FAILURE;
	}
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	void application_base::init()
	{
		device_ = video::create_device(video_config_);
		driver_ = device_->videoDriver();

		initImpl();
	}
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	void application_base::exit()
	{
		device_->destroy();
	}
	//----------------------------------------------------------------------------------------------

} /*tests*/ } /*djah*/