namespace djah { namespace video { namespace drivers {

	//----------------------------------------------------------------------------------------------
	template<typename OglImpl>
	opengl_base<OglImpl>::opengl_base(device_base *device)
		: driver_base(device)
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename OglImpl>
	opengl_base<OglImpl>::~opengl_base()
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename OglImpl>
	void opengl_base<OglImpl>::create()
	{
		OglImpl::create(device_);

		// Init glew
		//glewInit();

		// Init caps
		/*ogl::capabilities::init();

		glClearDepth(1.f);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);*/
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename OglImpl>
	void opengl_base<OglImpl>::destroy()
	{
		OglImpl::destroy();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename OglImpl>
	void opengl_base<OglImpl>::beginScene()
	{/*
		if( proj_dirty_ )
		{
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glMultMatrixf(projection_matrix_.data);
			proj_dirty_ = false;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMultMatrixf(view_matrix_.data);
*/
		// Use current shader
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename OglImpl>
	void opengl_base<OglImpl>::endScene()
	{
		// End using current shader
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename OglImpl>
	void opengl_base<OglImpl>::updateViewport()
	{
		const math::vector2i &topLeft = viewport_.topLeft();
		//glViewport(topLeft.x, topLeft.y, viewport_.width(), viewport_.height());
	}
	//----------------------------------------------------------------------------------------------

} /*drivers*/ } /*video*/ } /*djah*/