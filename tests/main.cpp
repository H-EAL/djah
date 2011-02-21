#include <cmath>

#include <djah/video/video_config.hpp>
#include <djah/video/device_base.hpp>
#include <djah/video/driver_base.hpp>
#include <djah/video/projection.hpp>
#include <djah/video/drivers/opengl_include.hpp>

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

private:

	math::vector3f eye_;
	math::vector3f center_;
	math::vector3f up_;

	math::matrix4f matPerspectiveProj_;
	math::matrix4f matOrthoProj_;
	math::matrix4f matView_;

	void init()
	{
		const float w = static_cast<float>(device_->videoConfig().width);
		const float h = static_cast<float>(device_->videoConfig().height);
		matPerspectiveProj_ = video::make_perspective_projection(60.0f, w/h, 0.1f, 1000.f);
		matOrthoProj_ = video::make_orthographic_projection(0.0f, w, h, 0.0f, -1.0f, 1.0f);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0,w, h,0, -1,1);

		math::matrix4f m;
		glGetFloatv(GL_PROJECTION_MATRIX, m.data);
		
		std::cout << matOrthoProj_.getTransposed() << std::endl;
		std::cout << m;

		//glMultMatrixf(matOrthoProj_.getTransposed().data);
	}

	void run()
	{		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		draw3D();
		draw2D();
		
		driver_->swapBuffers();
	}

	void exit()
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
	}

	void draw2D()
	{
		const float w = static_cast<float>(device_->videoConfig().width);
		const float h = static_cast<float>(device_->videoConfig().height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//glOrtho(0,w, h,0, -1,1);
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
};


int main()
{
	application app;
	app.exec();

	return 0;
}