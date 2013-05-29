#ifndef DJAH_BUMP_MAPPING_TEST_HPP
#define DJAH_BUMP_MAPPING_TEST_HPP

#include "test.hpp"
#include "djah/opengl.hpp"
#include "camera.hpp"
#include "djah/system/input/gamepad.hpp"
#include "djah/3d/shader.hpp"
#include "resource_finder.hpp"

class BumpMappingTest
	: public test_base
{
public:
	BumpMappingTest(djah::system::device_sptr pDevice, const djah::system::input::gamepad &g, Camera &cam);
	virtual ~BumpMappingTest();
	virtual void onInit();
	virtual void onExit();
	virtual void update(float dt);
	virtual void draw();
	virtual const char* name() const { return "Bump Mapping"; }

private:
	Camera &cam_;
	const djah::system::input::gamepad &gamepad_;

	djah::math::matrix4f matPerspectiveProj_;

	djah::opengl::vertex_buffer  *pVB_;
	djah::opengl::vertex_array   va_;
	djah::d3d::texture_ptr	pDiffuse_;
	djah::d3d::texture_ptr	pNormalMap_;
	djah::d3d::shader		shader_;


	djah::opengl::index_buffer   *pPlaneIB_;
	djah::opengl::vertex_buffer  *pPlaneVB_;
	djah::opengl::vertex_array   planeVA_;
	djah::d3d::texture_ptr		 pPlaneDiffuse_;
	djah::d3d::texture_ptr		 pPlaneNormalMap_;
};

#endif /* DJAH_BUMP_MAPPING_TEST_HPP */