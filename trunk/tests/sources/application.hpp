#ifndef DJAH_TESTS_APPLICATION_HPP
#define DJAH_TESTS_APPLICATION_HPP

#include <djah/math.hpp>
#include <djah/filesystem/memory_stream.hpp>
#include <djah/video/text.hpp>

#include "application_base.hpp"

class application
	: public djah::tests::application_base
{
public:

	explicit application(int w = 960, int h = 640);

private:

	virtual void initImpl();
	virtual void runImpl();
	virtual void exitImpl();

	void draw3D();
	void draw2D();

	void drawAxis();
	void drawMeshes();

	djah::math::vector3f eye_;
	djah::math::vector3f center_;
	djah::math::vector3f up_;

	djah::math::matrix4f matPerspectiveProj_;
	djah::math::matrix4f matOrthoProj_;
	djah::math::matrix4f matView_;

	struct mesh *cthulhu_;
	struct mesh *astroboy_;

	djah::video::text fps_str_;
};

#endif /* DJAH_TESTS_APPLICATION_HPP */