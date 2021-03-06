#include "djah/application_base.hpp"

#include "djah/system/device.hpp"
#include "djah/system/driver_base.hpp"


int main(int argc, char *argv[])
{
	djah::application_base *app = djah_new_app();
	int retVal = app->exec(argc, argv);
	delete app;
	return retVal;
}


namespace djah {

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
		(void)argc;
		(void)argv;

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
		exitImpl();

		if( device_ )
			device_->destroy();
	}
	//----------------------------------------------------------------------------------------------

} /*djah*/