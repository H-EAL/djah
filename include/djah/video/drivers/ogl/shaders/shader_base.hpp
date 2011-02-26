#ifndef DJAH_VIDEO_DRIVERS_OGL_SHADER_BASE_HPP
#define DJAH_VIDEO_DRIVERS_OGL_SHADER_BASE_HPP

#include <string>
#include "../resource.hpp"

namespace djah { namespace video { namespace drivers { namespace ogl {

	template<int ShaderType>
	class shader_base
		: public resource
	{
	public:

		explicit shader_base(const std::string &url = "");
		~shader_base();

		void loadSourceFromString(const std::string &source);
		void loadSourceFromUrl(const std::string &url);
		void compile() const;

	protected:

		void aquire();
		void release();
		bool isValidResource() const;

		void loadSource() const;
		void handleCompilationErrors() const;

		std::string source_;
	};

} /*ogl*/ } /*drivers*/ } /*video*/ } /*djah*/

#include "shader_base.inl"

#endif /* DJAH_VIDEO_DRIVERS_OGL_SHADER_BASE_HPP */