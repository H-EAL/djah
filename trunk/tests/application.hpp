#ifndef DJAH_TESTS_APPLICATION_HPP
#define DJAH_TESTS_APPLICATION_HPP

#include <djah/video/video_config.hpp>
#include <djah/video/device_base.hpp>
#include <djah/video/driver_base.hpp>
#include <djah/video/projection.hpp>
#include <djah/video/drivers/opengl_include.hpp>
#include <djah/video/drivers/ogl.hpp>

#include <djah/log/logger.hpp>
#include <djah/log/console_logger.hpp>

#include <djah/math.hpp>

#include "application_base.hpp"

using namespace djah;

class application
	: public djah::tests::application_base
{
public:

	explicit application(int w = 480, int h = 320)
		: application_base(djah::video::video_config(w,h))
		, eye_(1,1,math::pi_over_2)
		, center_(0,0,0)
		, up_(0,1,0)
	{
	}

	int exec(int argc = 0, char *argv[] = 0)
	{
		init();
		int retVal = djah::tests::application_base::exec(argc, argv);
		exit();
		return retVal;
	}

private:

	math::vector3f eye_;
	math::vector3f center_;
	math::vector3f up_;

	math::matrix4f matPerspectiveProj_;
	math::matrix4f matOrthoProj_;
	math::matrix4f matView_;

	virtual void initImpl()
	{
		djah::log::logger::setLogger(new djah::log::console_logger);
		const float w = static_cast<float>(device_->videoConfig().width);
		const float h = static_cast<float>(device_->videoConfig().height);
		matPerspectiveProj_ = video::make_perspective_projection(60.0f, w/h, 0.1f, 1000.f);
		matOrthoProj_ = video::make_orthographic_projection(0.0f, w, h, 0.0f, -1.0f, 1.0f);
		
		glClearDepth(1.f);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	virtual void runImpl()
	{		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		draw3D();
		//draw2D();
		
		driver_->swapBuffers();
	}

	void exitImpl()
	{
	}

	void draw3D()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMultMatrixf(matPerspectiveProj_.getTransposed().data);

		static float angle = 0.0f;
		eye_.x = cos(angle);
		eye_.z = sin(angle);
		angle = (angle+0.01f > math::pi_times_2) ? 0.0f : angle+0.01f;
		math::matrix4f matView = video::make_look_at(eye_, center_, up_);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMultMatrixf(matView.getTransposed().data);

		drawAxis();
		drawSomething();
	}

	void draw2D()
	{
		const float w = static_cast<float>(device_->videoConfig().width);
		const float h = static_cast<float>(device_->videoConfig().height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMultMatrixf(matOrthoProj_.getTransposed().data);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glBegin(GL_TRIANGLE_STRIP);
		{
			glColor4f(0.9f, 0.6f, 0.2f, 1.0f);
			glVertex2f(0,0);
			glVertex2f(w,0);
			glColor4f(0.9f, 0.6f, 0.25f, 0.7f);
			glVertex2f(0,h/2);
			glVertex2f(w,h/2);
		}
		glEnd();
	}

	void drawAxis()
	{
		glBegin(GL_LINES);
		{
			//X
			glColor3fv(math::vector3f::x_axis.data);
			glVertex3fv(math::vector3f::null_vector.data);
			glVertex3fv(math::vector3f::x_axis.data);
			//Y
			glColor3fv(math::vector3f::y_axis.data);
			glVertex3fv(math::vector3f::null_vector.data);
			glVertex3fv(math::vector3f::y_axis.data);
			//Z
			glColor3fv(math::vector3f::z_axis.data);
			glVertex3fv(math::vector3f::null_vector.data);
			glVertex3fv(math::vector3f::z_axis.data);
		}
		glEnd();
	}

	void drawSomething()
	{

			DJAH_TEST_FOR_OPENGL_ERRORS(int a);
		static const GLfloat CubeArray[] = {
			0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	-1.0f, -1.0f, -1.0f,
			0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,	-1.0f,  1.0f, -1.0f,
			1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,	 1.0f, -1.0f,  0.0f,
			1.0f, 1.0f, 0.0f,	1.0f, 1.0f, 0.0f,	 1.0f,  1.0f, -1.0f,
			1.0f, 0.0f, 1.0f,	1.0f, 0.0f, 1.0f,	 1.0f, -1.0f,  1.0f,
			1.0f, 1.0f, 1.0f,	1.0f, 1.0f, 1.0f,	 1.0f,  1.0f,  1.0f,
			0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,	-1.0f, -1.0f,  1.0f,
			0.0f, 1.0f, 1.0f,	0.0f, 1.0f, 1.0f,	-1.0f,  1.0f,  1.0f
		};

		static const GLuint IndiceArray[] = {
			0,1,2,1,3,2,2,3,4,3,5,4,4,5,6,5,7,6,6,7,0,7,1,0,1,7,3,7,5,3,5,6,2,6,4,2
		};

		static video::drivers::ogl::vertex_format vf;
		vf.record() << video::drivers::ogl::format::position<3,float>() <<  video::drivers::ogl::format::color<3,float>() <<  video::drivers::ogl::format::normal<3,float>();

		static video::drivers::ogl::vertex_buffer vb( sizeof(CubeArray),   video::drivers::ogl::EBU_STATIC_DRAW);
		static video::drivers::ogl::index_buffer  ib( sizeof(IndiceArray), video::drivers::ogl::EBU_STATIC_DRAW, video::drivers::ogl::EDM_TRIANGLES );
		
		static video::drivers::ogl::vertex_array  va(vf, &vb, &ib);
		static video::drivers::ogl::shader_program sp;

		static bool initialized = false;
		if( !initialized )
		{
			vb.write(CubeArray);
			ib.write(IndiceArray);

			video::drivers::ogl::vertex_shader vs;
			video::drivers::ogl::pixel_shader ps;

			const char *vsstr = "uniform mat4 ProjectionMatrix; attribute vec4 Position;\
								void main() { gl_Position = ProjectionMatrix * Position; }";

			const char *psstr = "void main() { gl_FragColor = vec4(1,0,0,1); }";

			vs.loadSourceFromString(vsstr);
			vs.compile();
			ps.loadSourceFromString(psstr);
			ps.compile();
			sp.attach(vs);
			sp.attach(ps);
			sp.link();

			va.init(sp);
			initialized = true;
		}

		sp.begin();
		sp.sendUniformMatrix("ProjectionMatrix", matPerspectiveProj_.getTransposed());
		va.draw();
		sp.end();
	}
};

#endif /* DJAH_TESTS_APPLICATION_HPP */