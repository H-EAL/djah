#ifndef DJAH_SYSTEM_CONTEXT_HPP
#define DJAH_SYSTEM_CONTEXT_HPP

#include <memory>
#include "video_config.hpp"

namespace djah { namespace system {

	class context_impl;

	class gl_context
	{
	public:
		gl_context(const gl_format &_format);
		~gl_context();

		bool create();
		void makeCurrent();

		static gl_context*	get_current();
		static void			done_current();

	private:
		std::unique_ptr<context_impl>	pImpl_;
		bool							initialized_;
		gl_format						format_;

	private:
		static gl_context *sp_current_context_;
	};

} /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_CONTEXT_HPP */