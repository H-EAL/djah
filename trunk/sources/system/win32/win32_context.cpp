#include "system/context.hpp"
#include "system/device.hpp"
#include "system/gl.hpp"
#include "debug/assertion.hpp"
#include "wgl_extensions.hpp"
#include "types.hpp"

namespace djah { namespace system {

	//----------------------------------------------------------------------------------------------
	class context_impl
	{
	public:
		context_impl()
			: hGLRC_(nullptr)
		{

		}

		~context_impl()
		{
			wglDeleteContext(hGLRC_);
			hGLRC_ = nullptr;
			hDC_   = nullptr;
		}

		bool create(const gl_format &f)
		{
			DJAH_ASSERT( device::get_current() );

			hDC_ = GetDC( (HWND)device::get_current()->windowHandle() );
			DJAH_ASSERT( hDC_ != INVALID_HANDLE_VALUE );

			int flags = WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;
			if( f.enableDebug )
				flags |= WGL_CONTEXT_DEBUG_BIT_ARB;
			int mask = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;

			const int attributes[] =
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, f.majorVersion,
				WGL_CONTEXT_MINOR_VERSION_ARB, f.minorVersion,
				WGL_CONTEXT_FLAGS_ARB, flags,
				WGL_CONTEXT_PROFILE_MASK_ARB, mask,
				0
			};

			hGLRC_ = wglCreateContextAttribsARB(hDC_, nullptr, attributes);

			return (hGLRC_ != 0);
		}

		HDC   hDC_;
		HGLRC hGLRC_;
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	gl_context *gl_context::sp_current_context_ = nullptr;
	//----------------------------------------------------------------------------------------------
	gl_context* gl_context::get_current()
	{
		return sp_current_context_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	gl_context::gl_context(const gl_format &_format)
		: pImpl_( new context_impl )
		, initialized_(false)
		, format_(_format)
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	gl_context::~gl_context()
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool gl_context::create()
	{
		return pImpl_->create(format_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void gl_context::makeCurrent()
	{
		wglMakeCurrent(pImpl_->hDC_, pImpl_->hGLRC_);
		sp_current_context_ = this;
		initialized_ = true;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void gl_context::doneCurrent()
	{
		wglMakeCurrent(0,0);
		sp_current_context_ = nullptr;
	}
	//----------------------------------------------------------------------------------------------

} /*system*/ } /*djah*/