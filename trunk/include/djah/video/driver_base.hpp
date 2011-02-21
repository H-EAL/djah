#ifndef DJAH_VIDEO_DRIVER_BASE_HPP
#define DJAH_VIDEO_DRIVER_BASE_HPP

#include "../math/matrix4.hpp"
#include "../geometry/rect.hpp"

namespace djah { namespace video {

	// Forward declaration
	class device_base;
	
	//----------------------------------------------------------------------------------------------
	class driver_base
	{
	public:

		driver_base(device_base *device);
		virtual ~driver_base();

		virtual void create() = 0;
		virtual void destroy() = 0;
		virtual void swapBuffers() = 0;

		virtual void beginScene() = 0;
		virtual void endScene() = 0;

		void setViewport(const geometry::rect_i &viewport);
		const geometry::rect_i& getViewport() const;

		const math::matrix4f& getViewMatrix() const;
		void setViewMatrix(const math::matrix4f &proj_mat);
		const math::matrix4f& getProjectionMatrix() const;
		void setProjectionMatrix(const math::matrix4f &proj_mat);

	protected:

		virtual void updateViewport() = 0;

		device_base			*device_;

		geometry::rect_i	viewport_;
		math::matrix4f		view_matrix_;
		math::matrix4f		projection_matrix_;

		// Indicates wether the projection matrix has been modified or not
		bool				proj_dirty_;
	};
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	// Pointer to a driver : should be reference counted
	//----------------------------------------------------------------------------------------------
	typedef driver_base* driver_ptr;
	//----------------------------------------------------------------------------------------------

} /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_DRIVER_BASE_HPP */