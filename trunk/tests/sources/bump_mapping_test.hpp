#ifndef DJAH_BUMP_MAPPING_TEST_HPP
#define DJAH_BUMP_MAPPING_TEST_HPP

#include "test.hpp"
#include "djah/opengl.hpp"
#include "camera.hpp"
#include "djah/system/input/gamepad.hpp"

class BumpMappingTest
	: public test_base
{
public:
	BumpMappingTest(djah::system::device_ptr pDevice, const djah::system::input::gamepad &g, Camera &cam);
	virtual ~BumpMappingTest();
	virtual void update(float dt);
	virtual void draw();
	virtual const char* name() const { return "Bump Mapping"; }

private:
	Camera &cam_;
	const djah::system::input::gamepad &gamepad_;

	djah::math::matrix4f matPerspectiveProj_;

	djah::opengl::vertex_buffer  *pVB_;
	djah::opengl::vertex_array   *pVA_;
	djah::opengl::texture		 *pDiffuse_;
	djah::opengl::texture		 *pNormalMap_;
	djah::opengl::shader_program shader_;


	djah::opengl::index_buffer   *pPlaneIB_;
	djah::opengl::vertex_buffer  *pPlaneVB_;
	djah::opengl::vertex_array   *pPlaneVA_;
	djah::opengl::texture		 *pPlaneDiffuse_;
	djah::opengl::texture		 *pPlaneNormalMap_;
};

#endif /* DJAH_BUMP_MAPPING_TEST_HPP */