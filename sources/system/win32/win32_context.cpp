#include "djah/system/context.hpp"
#include "djah/system/device.hpp"
#include "djah/system/video_config.hpp"
#include "djah/system/gl.hpp"
#include "djah/debug/assertion.hpp"
#include "djah/types.hpp"
#include "./wgl_extensions.hpp"

namespace djah { namespace system {

	//----------------------------------------------------------------------------------------------
	class context_impl
	{
	public:
		context_impl()
			: hDC_(nullptr)
			, hGLRC_(nullptr)
		{

		}

		~context_impl()
		{
			wglDeleteContext(hGLRC_);
			hGLRC_ = nullptr;
			hDC_   = nullptr;
		}

		void createTemporaryContext(const device_sptr &pDevice)
		{
			hDC_ = GetDC( pDevice->handle<HWND>() );
			check(hDC_);

			hGLRC_ = wglCreateContext(hDC_);
			check(hGLRC_);

			wglMakeCurrent(hDC_, hGLRC_);
			load_wgl_extensions();

			glGetIntegerv(GL_MAJOR_VERSION, &driver_config::s_default_major_version);
			glGetIntegerv(GL_MINOR_VERSION, &driver_config::s_default_minor_version);
		}

		bool create(const device_sptr &pDevice, const driver_config_sptr &pConfig)
		{
			int majorVersion = pConfig->majorVersion;
			int minorVersion = pConfig->minorVersion;

			if( pConfig->majorVersion == 0 )
			{
				majorVersion = driver_config::s_default_major_version;
				minorVersion = driver_config::s_default_minor_version;
			}

			check(majorVersion > 0);

			hDC_ = GetDC( pDevice->handle<HWND>() );
			check(hDC_);

			int flags = pConfig->enableCompatibilityProfile ? WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB : 0;
			if( pConfig->enableDebug )
				flags |= WGL_CONTEXT_DEBUG_BIT_ARB;

			const int profileMask = pConfig->enableCompatibilityProfile
								  ? WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB
								  : WGL_CONTEXT_CORE_PROFILE_BIT_ARB;

			const int attributes[] =
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB,	majorVersion,
				WGL_CONTEXT_MINOR_VERSION_ARB,	minorVersion,
				WGL_CONTEXT_FLAGS_ARB,			flags,
				WGL_CONTEXT_PROFILE_MASK_ARB,	profileMask,
				0
			};

			hGLRC_ = wglCreateContextAttribsARB(hDC_, nullptr, attributes);

			return (hGLRC_ != nullptr);
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
	gl_context::gl_context(const device_sptr &pDevice, const  driver_config_sptr &_pConfig)
		: successfullyCreated_(false)
		, pConfig_(_pConfig)
		, pImpl_( new context_impl )
	{
		if( sp_current_context_ == nullptr )
		{
			pImpl_->createTemporaryContext(pDevice);
			successfullyCreated_ = true;
			makeCurrent();
		}
		else
		{
			successfullyCreated_ = pImpl_->create(pDevice, pConfig_);
			setVSync(pConfig_->vsync);
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	gl_context::~gl_context()
	{
		if( sp_current_context_ == this )
		{
			done_current();
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void gl_context::makeCurrent()
	{
		check(successfullyCreated_);
		wglMakeCurrent(pImpl_->hDC_, pImpl_->hGLRC_);
		sp_current_context_ = this;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void gl_context::done_current()
	{
		wglMakeCurrent(nullptr, nullptr);
		sp_current_context_ = nullptr;
	}
	//----------------------------------------------------------------------------------------------


	//-------------------------------------------------------------------------------------------------
	void gl_context::setVSync(bool enabled)
	{
		wglSwapIntervalEXT(enabled ? 1 : 0);
	}
	//-------------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void gl_context::swapBuffers()
	{
		SwapBuffers(pImpl_->hDC_);
	}
	//----------------------------------------------------------------------------------------------

} /*system*/ } /*djah*/