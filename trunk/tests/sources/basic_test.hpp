#ifndef DJAH_BASIC_TEST_HPP
#define DJAH_BASIC_TEST_HPP

#include "test.hpp"
#include "djah/opengl.hpp"
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
	void initPrimitive(const std::string &vsFile, const std::string &psFile, djah::opengl::shader_program &sp, djah::opengl::vertex_buffer *&pVB, djah::opengl::vertex_array *&pVA, const std::string &textureFile, djah::opengl::texture *&pTex);

private:
	Camera &cam_;

	djah::math::matrix4f matPerspectiveProj_;

	djah::opengl::vertex_buffer  *pCubeVB_;
	djah::opengl::vertex_array   *pCubeVA_;
	djah::opengl::texture		 *pCubeTexture_;
	djah::opengl::shader_program cubeShader_;

	djah::opengl::vertex_buffer  *pSphereVB_;
	djah::opengl::vertex_array   *pSphereVA_;
	djah::opengl::texture		 *pSphereTexture_;
	djah::opengl::shader_program sphereShader_;
};

#endif /* DJAH_BASIC_TEST_HPP */