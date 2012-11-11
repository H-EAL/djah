#ifndef DJAH_FONT_TEST_HPP
#define DJAH_FONT_TEST_HPP

#include "test.hpp"
#include "djah/opengl.hpp"
#include "camera.hpp"

class FontTest
	: public test_base
{
public:
	FontTest(djah::system::device_ptr pDevice, Camera &cam);
	virtual ~FontTest();
	virtual void update(float dt);
	virtual void draw();
	virtual const char* name() const { return "Font"; }

private:
	void initPlane(const std::string &str, const math::vector2f &position = math::vector2f::null_vector);
	void initFont(const std::string &fontFile, int fontSize);

private:
	Camera &cam_;

	djah::math::matrix4f matOrthoProj_;

	djah::opengl::index_buffer   *pIB_;
	djah::opengl::vertex_buffer  *pVB_;
	djah::opengl::vertex_array   *pVA_;
	djah::opengl::texture		 *pTexture_;
	djah::opengl::shader_program shader_;
	djah::opengl::shader_program colorShader_;
	djah::opengl::vertex_buffer  *pLinesVB_;
	djah::opengl::vertex_array   *pLinesVA_;
};

#endif /* DJAH_FONT_TEST_HPP */