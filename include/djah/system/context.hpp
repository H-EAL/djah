#ifndef DJAH_SYSTEM_CONTEXT_HPP
#define DJAH_SYSTEM_CONTEXT_HPP

#include <memory>

namespace djah { namespace system {

	//----------------------------------------------------------------------------------------------
	// Forward declarations
	//----------------------------------------------------------------------------------------------
	class device;
	class driver_config;
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	// OpenGL context linked to a driver
	//----------------------------------------------------------------------------------------------
	class gl_context
	{
	public:
		gl_context(const std::shared_ptr<device> &_pDevice, const std::shared_ptr<driver_config> &_pConfig);
		~gl_context();

		void setVSync(bool enabled);
		void swapBuffers();

		bool isValid() const { return successfullyCreated_; }

		void makeCurrent();

		static gl_context* get_current();
		static void		   done_current();

	private:
		bool								successfullyCreated_;
		std::shared_ptr<driver_config>		pConfig_;
		std::unique_ptr<class context_impl>	pImpl_;

	private:
		static gl_context *sp_current_context_;
	};
	//----------------------------------------------------------------------------------------------

} /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_CONTEXT_HPP */