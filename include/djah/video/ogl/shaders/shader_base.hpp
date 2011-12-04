#ifndef DJAH_VIDEO_OGL_SHADER_BASE_HPP
#define DJAH_VIDEO_OGL_SHADER_BASE_HPP

#include <string>

#include "../resource.hpp"
#include "../../../log/logger.hpp"

namespace djah { namespace video { namespace ogl {

	template<int ShaderType>
	class shader_base
		: public resource
	{
	public:

		explicit shader_base(const std::string &source = "");
		virtual ~shader_base();

		void loadSource(const std::string &source);
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
