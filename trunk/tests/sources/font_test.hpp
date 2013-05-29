#ifndef DJAH_FONT_TEST_HPP
#define DJAH_FONT_TEST_HPP

#include "test.hpp"
#include "djah/opengl.hpp"
#include "djah/3d/shader.hpp"
#include "camera.hpp"

class FontTest
	: public test_base
{
public:
	FontTest(djah::system::device_sptr pDevice, Camera &cam);
	virtual ~FontTest();
	virtual void onInit();
	virtual void onExit();
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
	djah::opengl::vertex_array   vA_;
	djah::opengl::texture		 *pTexture_;

	djah::d3d::shader textShader_;
	djah::d3d::shader colorShader_;
};

#endif /* DJAH_FONT_TEST_HPP */