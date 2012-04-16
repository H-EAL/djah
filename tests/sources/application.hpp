#ifndef DJAH_TESTS_APPLICATION_HPP
#define DJAH_TESTS_APPLICATION_HPP

#include <djah/math.hpp>
#include <djah/filesystem/memory_stream.hpp>
#include <djah/video/text.hpp>
#include <djah/system/input/mouse.hpp>
#include <djah/system/input/keyboard.hpp>
#include <djah/system/input/gamepad.hpp>

#include <djah/application_base.hpp>
#include "camera.hpp"


class application
	: public djah::application_base
{
public:

	explicit application(int w = 1280, int h = 720);

private:

	virtual void initImpl();
	virtual void runImpl();
	virtual void exitImpl();

	void draw3D();
	void draw2D();

	void drawAxis();
	void drawMeshes();

	djah::math::vector3f eye_;
	djah::math::vector3f center_;
	djah::math::vector3f up_;

	djah::math::matrix4f matPerspectiveProj_;
	djah::math::matrix4f matOrthoProj_;
	djah::math::matrix4f matView_;

	struct mesh *cthulhu_;
	struct mesh *astroboy_;
	struct mesh *cow_;
	struct mesh *dude_;
	Camera cam;

	djah::system::input::mouse		mouse_;
	djah::system::input::keyboard	keyboard_;
	djah::system::input::gamepad	gamepad_;

	djah::video::text fps_str_;
	djah::video::text mouse_pos_;
};

DJAH_ENTRY_POINT(application);

#endif /* DJAH_TESTS_APPLICATION_HPP */