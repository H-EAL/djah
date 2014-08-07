#ifndef DJAH_TESS_TEST_HPP
#define DJAH_TESS_TEST_HPP

#include "test.hpp"
#include "djah/opengl.hpp"
#include "djah/3d/shader.hpp"
#include "djah/system/input/keyboard.hpp"
#include "resource_finder.hpp"
#include "djah/gameplay/game_object.hpp"
#include "djah/gameplay/components.hpp"

typedef djah::gameplay::entity_t<djah::gameplay::components::DefaultComponentTypes> Entity;

class Camera;

class TesselationTest
	: public test_base
{
public:
	TesselationTest(djah::system::device_sptr pDevice, djah::system::input::keyboard &kb, Camera &cam);
	virtual ~TesselationTest();
	virtual void onInit();
	virtual void onExit();
	virtual void update(float dt);
	virtual void draw();
	virtual const char* name() const { return "Tesselation"; }

private:

private:
	Camera &cam_;
	djah::system::input::keyboard &kb_;

	djah::math::matrix4f matPerspectiveProj_;

	djah::opengl::vertex_buffer *pVB_;
	djah::opengl::vertex_array  vertArr_;
	djah::d3d::shader			shader_;
	djah::d3d::texture_sptr		pTexture_;

	Entity pGO;
};

#endif /* DJAH_TESS_TEST_HPP */