#include "djah/system/opengl_driver.hpp"
#include "djah/system/device.hpp"
#include "djah/system/gl.hpp"
#include "djah/opengl/capabilities.hpp"
#include "djah/debug/assertion.hpp"
#include "djah/math/vector2.hpp"

namespace djah { namespace system {

	//----------------------------------------------------------------------------------------------
    opengl_driver::opengl_driver()
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	opengl_driver::~opengl_driver()
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void opengl_driver::create()
	{
		DJAH_ASSERT( glContext_.create() );
		glContext_.makeCurrent();
		device::get_current()->deleteTempContext();
		load_extensions();
		opengl::capabilities::init();
	}
	//----------------------------------------------------------------------------------------------

} /*system*/ } /*djah*/
