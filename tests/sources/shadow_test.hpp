#ifndef DJAH_SHADOW_TEST_HPP
#define DJAH_SHADOW_TEST_HPP

#include "test.hpp"
#include <djah/math.hpp>

#include <djah/filesystem/memory_stream.hpp>

#include <djah/video/text.hpp>

#include <djah/opengl.hpp>

#include <djah/system/device.hpp>
#include <djah/system/input/mouse.hpp>
#include <djah/system/input/keyboard.hpp>
#include <djah/system/input/gamepad.hpp>

#include <djah/dataobject/global_registry.hpp>

#include "camera.hpp"

class ShadowTest
	: public test_base
{
public:
	ShadowTest(djah::system::device_ptr pDevice, 
		djah::system::input::mouse    &m, 
		djah::system::input::keyboard &k,
		djah::system::input::gamepad  &g,
		Camera &c);

	virtual ~ShadowTest();
	virtual void update(float dt);
	virtual void draw();
	virtual const char* name() const { return "Shadow Mapping"; }

private:
	void init();
	void drawAxis(bool);
	void drawMeshes();

private:
	djah::math::vector3f spotPosition_;
	djah::math::vector3f spotDirection_;
	djah::math::matrix4f matSpotView_;

	djah::math::matrix4f matPerspectiveProj_;
	djah::math::matrix4f matShadowPerspectiveProj_;
	djah::math::matrix4f matView_;

	struct model *cthulhu_;
	struct model *astroboy_;
	struct model *cow_;
	struct model *dude_;
	Camera &cam;

	djah::system::input::mouse		&mouse_;
	djah::system::input::keyboard	&keyboard_;
	djah::system::input::gamepad	&gamepad_;


	djah::opengl::frame_buffer	*pFBO_;
	djah::opengl::texture		*pShadowMap_;
	djah::opengl::texture		*pFloorTexture_;
	djah::opengl::vertex_array  *pVertexArray_;
	djah::opengl::vertex_buffer *pVertexBuffer_;
};

#endif /* DJAH_SHADOW_TEST_HPP */