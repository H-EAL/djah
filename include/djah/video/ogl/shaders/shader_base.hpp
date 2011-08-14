#ifndef DJAH_VIDEO_OGL_SHADER_BASE_HPP
#define DJAH_VIDEO_OGL_SHADER_BASE_HPP

#include <string>

#include <boost/smart_ptr/scoped_array.hpp>

#include "../resource.hpp"
#include "../../../log/logger.hpp"
#include "../../../filesystem/browser.hpp"

namespace djah { namespace video { namespace ogl {

	template<int ShaderType>
	class shader_base
		: public resource
	{
	public:

		explicit shader_base(const std::string &url = "");
		virtual ~shader_base();

		void loadSourceFromString(const std::string &source);
		void loadSourceFromUrl(const std::string &url);
		void compile() const;

	protected:

		virtual void aquire();
		virtual void release();
		virtual bool isValidResource() const;

		void loadSource() const;
		void handleCompilationErrors() const;

		std::string source_;
	};

} /*ogl*/ } /*video*/ } /*djah*/

#include "shader_base.inl"

#endif /* DJAH_VIDEO_OGL_SHADER_BASE_HPP */
