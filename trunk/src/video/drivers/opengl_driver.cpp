#include "video/drivers/opengl_driver.hpp"
#include "video/drivers/opengl_include.hpp"
#include "video/drivers/detail/opengl_context.hpp"
#include "video/drivers/ogl/gl_caps.hpp"

namespace djah { namespace video { namespace drivers {

	//----------------------------------------------------------------------------------------------
	opengl_driver::opengl_driver(device_base *device)
		: driver_base(device)
		, context_(detail::new_platform_specific_context())
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

		// Init glew
		glewInit();

		// Init caps
		ogl::capabilities::init();

		glClearDepth(1.f);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
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
	detail::opengl_context* opengl_driver::context() const
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

} /*drivers*/ } /*video*/ } /*djah*/