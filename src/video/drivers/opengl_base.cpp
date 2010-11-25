#include "video/drivers/opengl_base.hpp"
#include <GL/glew.h>
#include "video/drivers/ogl/gl_caps.hpp"

namespace djah { namespace video { namespace drivers {

	//----------------------------------------------------------------------------------------------
	opengl_base::opengl_base(device_base *device)
		: driver_base(device)
	{
		view_matrix_.identity();
		projection_matrix_.identity();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	opengl_base::~opengl_base()
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void opengl_base::create()
	{
		OglImpl::create(device_);

		// Init glew
		//glewInit();

		// Init caps
		ogl::capabilities::init();

		glClearDepth(1.f);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void opengl_base::destroy()
	{
		OglImpl::destroy();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void opengl_base::beginScene()
	{
		if( proj_dirty_ )
		{
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glMultMatrixf(projection_matrix_.data());
			proj_dirty_ = false;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMultMatrixf(view_matrix_.data());

		// Use current shader
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void opengl_base::endScene()
	{
		// End using current shader
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void opengl_base::updateViewport()
	{
		const math::vector2i &topLeft = viewport_.topLeft();
		glViewport(topLeft.x(), topLeft.y(), viewport_.width(), viewport_.height());
	}
	//----------------------------------------------------------------------------------------------

} /*drivers*/ } /*video*/ } /*djah*/