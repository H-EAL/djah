#include <djah/application_base.hpp>
#include <djah/math.hpp>

#include <djah/opengl/texture.hpp>
#include <djah/3d/text.hpp>

class viewer_app
	: public djah::application_base
{
public:

	viewer_app();
	virtual ~viewer_app();

protected:

	virtual void initImpl();
	virtual void runImpl();
	virtual void exitImpl();

private:

	class mesh_builder *builder_;

	djah::math::vector3f eye_;
	djah::math::vector3f center_;
	djah::math::vector3f up_;

	djah::math::matrix4f matPerspectiveProj_;
	djah::math::matrix4f matOrthoProj_;
	djah::math::matrix4f matView_;

	djah::opengl::texture *texture_;
	djah::d3d::text fps_str_;
};

DJAH_ENTRY_POINT(viewer_app);