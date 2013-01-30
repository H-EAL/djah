#ifndef DJAH_DEFERRED_SHADING_TEST_HPP
#define DJAH_DEFERRED_SHADING_TEST_HPP

#include "test.hpp"
#include "model.hpp"
#include "camera.hpp"
#include "djah/opengl.hpp"
#include "djah/system/input/gamepad.hpp"
#include "djah/3d/shader.hpp"
#include "resource_finder.hpp"

class DeferredShadingTest
	: public test_base
{
public:
	DeferredShadingTest(djah::system::device_ptr pDevice, const djah::system::input::gamepad &g, Camera &cam);
	virtual ~DeferredShadingTest();
	virtual void onInit();
	virtual void onExit();
	virtual void update(float dt);
	virtual void draw();
	virtual const char* name() const { return "Deferred Shading"; }

private:
	void renderGeometryPass(const math::matrix4f &matViewProj);
	void renderLightPass(const math::matrix4f &matViewProj);
	void renderFinalPass(const math::matrix4f &matViewProj);
	void renderTest(const math::matrix4f &matViewProj);

private:
	Camera &cam_;
	model cow_;

	const djah::system::input::gamepad &gamepad_;

	djah::math::matrix4f matPerspectiveProj_;
	djah::math::matrix4f matOrthoProj_;

	djah::opengl::vertex_buffer  *pVertexBuffer_;
	djah::opengl::vertex_array   vertexArray_;

	djah::d3d::shader		shaderProgram_;
	djah::d3d::shader		deferredProgram_;
	djah::d3d::shader		compositorProgram_;
	djah::d3d::texture_ptr	pFloorTexture_;


	enum eGeometryBuffer
	{
		eGB_Positions,
		eGB_Diffuse,
		eGB_Normals,
		eGB_TexCoords,
		eGB_Count
	};
	djah::opengl::frame_buffer  deferredFBO_;
	djah::opengl::texture		*pGeometryBuffers_[eGB_Count];
	djah::opengl::texture		depthFB_;
	int currentBuffer_;
};

#endif /* DJAH_DEFERRED_SHADING_TEST_HPP */