#include "application_base.hpp"

#include <djah/system/device_base.hpp>
#include <djah/system/driver_base.hpp>
#include <djah/system/device_base.hpp>

namespace djah { namespace tests {

	//----------------------------------------------------------------------------------------------
	application_base::application_base(const system::video_config &config)
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
		int retVal = EXIT_FAILURE;

		init();
		if( device_ && driver_ )
		{
			while( device_->run() )
			{
				runImpl();
				device_->swapBuffers();
			}
			retVal = EXIT_SUCCESS;
		}
		exit();

		return retVal;
	}
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	void application_base::init()
	{
		device_ = system::create_device(video_config_);
		driver_ = device_->videoDriver();

		if( device_ && driver_ )
			initImpl();
	}
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	void application_base::exit()
	{
		if( device_ )
			device_->destroy();
	}
	//----------------------------------------------------------------------------------------------

} /*tests*/ } /*djah*/