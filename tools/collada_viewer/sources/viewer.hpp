#include <djah/application_base.hpp>
#include <djah/math.hpp>

#include <djah/video/ogl/buffers/texture.hpp>
#include <djah/video/text.hpp>

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

	djah::video::ogl::texture *texture_;
	djah::video::text fps_str_;
};

DJAH_ENTRY_POINT(viewer_app);