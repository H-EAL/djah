#include "djah/system/context.hpp"
#include "djah/system/device.hpp"
#include "djah/system/video_config.hpp"
#include "djah/system/gl.hpp"
#include "djah/system/system_logger.hpp"
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

		void createTemporaryContext(device *pDevice)
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

		bool create(device *pDevice, const driver_config_sptr &pConfig, context_impl *pSharedContextImpl)
		{
			int majorVersion = pConfig->majorVersion;
			int minorVersion = pConfig->minorVersion;

			if( majorVersion == 0 )
			{
				majorVersion = driver_config::s_default_major_version;
				minorVersion = driver_config::s_default_minor_version;
			}

			check(majorVersion > 0);

			hDC_ = GetDC( pDevice->handle<HWND>() );
			check(hDC_);

			int flags = 0;//pConfig->enableCompatibilityProfile ? WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB : 0;
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

			HGLRC sharedHGLRC = pSharedContextImpl ? pSharedContextImpl->hGLRC_ : nullptr;
			hGLRC_ = wglCreateContextAttribsARB(hDC_, sharedHGLRC, attributes);

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
	gl_context::gl_context(device *pDevice, const  driver_config_sptr &_pConfig, gl_context *pSharedContext)
		: successfullyCreated_(false)
		, pConfig_(_pConfig)
		, pImpl_( new context_impl )
	{
		static bool firstContextCreation = true;
		if( firstContextCreation )
		{
			context_impl defaultContextImpl;
			defaultContextImpl.createTemporaryContext(pDevice);
			firstContextCreation = false;

			DJAH_SYSTEM_NOTIFICATION()
				<< "Temporary OpenGL context created (version "
				<< driver_config::s_default_major_version
				<< "."
				<< driver_config::s_default_minor_version
				<< ")" << DJAH_END_LOG();
		}

		context_impl *pSharedContextImpl = pSharedContext ? pSharedContext->pImpl_.get() : nullptr;
		successfullyCreated_ = pImpl_->create(pDevice, pConfig_, pSharedContextImpl);
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
		DJAH_SYSTEM_NOTIFICATION() << (enabled ? "Enabling" : "Disabling") << " VSync" << DJAH_END_LOG();
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