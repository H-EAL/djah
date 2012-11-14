#ifndef DJAH_BASIC_TEST_HPP
#define DJAH_BASIC_TEST_HPP

#include "test.hpp"
#include "djah/opengl.hpp"
#include "djah/3d/shader.hpp"
#include "camera.hpp"

class BasicTest
	: public test_base
{
public:
	BasicTest(djah::system::device_ptr pDevice, Camera &cam);
	virtual ~BasicTest();
	virtual void update(float dt);
	virtual void draw();
	virtual const char* name() const { return "Primitives"; }

private:
	template<typename P>
	void initPrimitive(djah::opengl::vertex_buffer *&pVB, djah::opengl::vertex_array *&pVA, const std::string &textureFile, djah::opengl::texture *&pTex, int level = 0);

private:
	Camera &cam_;

	djah::math::matrix4f matPerspectiveProj_;

	djah::opengl::vertex_buffer  *pCubeVB_;
	djah::opengl::vertex_array   *pCubeVA_;
	djah::opengl::texture		 *pCubeTexture_;

	djah::opengl::vertex_buffer  *pSphereVB_;
	djah::opengl::vertex_array   *pSphereVA_;
	djah::opengl::texture		 *pSphereTexture_;

	djah::opengl::vertex_buffer  *pCylinderVB_;
	djah::opengl::vertex_array   *pCylinderVA_;
	djah::opengl::texture		 *pCylinderTexture_;

	djah::opengl::vertex_buffer  *pConeVB_;
	djah::opengl::vertex_array   *pConeVA_;
	djah::opengl::texture		 *pConeTexture_;

	djah::d3d::shader shader_;
	djah::d3d::shader shaderColored_;
};

#endif /* DJAH_BASIC_TEST_HPP */