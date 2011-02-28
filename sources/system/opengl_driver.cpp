#include "system/opengl_driver.hpp"
#include "system/opengl_include.hpp"
#include "system/opengl_context.hpp"
#include "video/ogl/gl_caps.hpp"

namespace djah { namespace system {

	//----------------------------------------------------------------------------------------------
	opengl_driver::opengl_driver(device_base *device)
		: driver_base(device)
		, context_(new_platform_specific_context())
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
		context_->create(device_);

		video::ogl::capabilities::init();
		video::ogl::load_extensions();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void opengl_driver::destroy()
	{
		context_->destroy();
		delete context_;
		context_ = 0;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void opengl_driver::swapBuffers()
	{
		context_->swapBuffers();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	opengl_context* opengl_driver::context() const
	{
		return context_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void opengl_driver::beginScene()
	{
		if( proj_dirty_ )
		{
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glMultMatrixf(projection_matrix_.getTransposed().data);
			proj_dirty_ = false;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMultMatrixf(view_matrix_.getTransposed().data);

		// Use current shader
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void opengl_driver::endScene()
	{
		// End using current shader
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void opengl_driver::updateViewport()
	{
		const math::vector2i &topLeft = viewport_.topLeft();
		glViewport(topLeft.x, topLeft.y, viewport_.width(), viewport_.height());
	}
	//----------------------------------------------------------------------------------------------

} /*system*/ } /*djah*/
