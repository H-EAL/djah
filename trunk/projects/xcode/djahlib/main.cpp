#include <iostream>
#include "djah/video/device_base.hpp"

int main()
{
	djah::video::device_ptr device = djah::video::create_device(480,320);
	
	while(device->run())
	{
	}
	
	return EXIT_SUCCESS;
}