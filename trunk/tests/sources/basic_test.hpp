#ifndef DJAH_BASIC_TEST_HPP
#define DJAH_BASIC_TEST_HPP

#include "test.hpp"
#include "djah/opengl.hpp"
#include "djah/3d/shader.hpp"
#include "camera.hpp"
#include "resource_finder.hpp"
#include "djah/3d/primitives/batcher.hpp"

class BasicTest
	: public test_base
{
public:
	BasicTest(djah::system::device_sptr pDevice, Camera &cam);
	virtual ~BasicTest();
	virtual void onInit();
	virtual void onExit();
	virtual void update(float dt);
	virtual void draw();
	virtual const char* name() const { return "Primitives"; }

private:
	template<typename P>
	void initPrimitive(djah::opengl::vertex_buffer *&pVB, djah::opengl::vertex_array &pVA, const std::string &textureFile, djah::d3d::texture_ptr &pTex, int level1 = 0, int level2 = 0);

	void drawTranslationGizmo(const math::matrix4f &matVP = math::matrix4f::identity);
	void drawRotationGizmo(const math::matrix4f &world = math::matrix4f::identity);
	void drawInstanced(const math::matrix4f &matVP);

private:
	Camera &cam_;

	djah::math::matrix4f matPerspectiveProj_;

	djah::opengl::vertex_buffer  *pCubeVB_;
	djah::opengl::vertex_array   cubeVA_;
	djah::d3d::texture_ptr		 pCubeTexture_;

	djah::opengl::vertex_buffer  *pSphereVB_;
	djah::opengl::vertex_array   sphereVA_;
	djah::d3d::texture_ptr		 pSphereTexture_;

	djah::opengl::vertex_buffer  *pCylinderVB_;
	djah::opengl::vertex_array   cylinderVA_;
	djah::d3d::texture_ptr		 pCylinderTexture_;

	djah::opengl::vertex_buffer  *pConeVB_;
	djah::opengl::vertex_array   coneVA_;
	djah::d3d::texture_ptr		 pConeTexture_;

	djah::opengl::vertex_buffer  *pRibbonVB_;
	djah::opengl::vertex_array   ribbonVA_;
	djah::d3d::texture_ptr		 pRibbonTexture_;

	djah::d3d::shader shader_;
	djah::d3d::shader shaderColored_;
	djah::d3d::shader shaderBatch_;

	djah::d3d::primitives::batcher batcher_;

	djah::resources::mesh_ptr	pBlockMesh;
	djah::resources::mesh_ptr	pGridMesh;
	djah::d3d::texture_ptr		pCowTexture_;
};

#endif /* DJAH_BASIC_TEST_HPP */