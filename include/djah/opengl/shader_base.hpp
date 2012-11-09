#ifndef DJAH_OPENGL_SHADER_BASE_HPP
#define DJAH_OPENGL_SHADER_BASE_HPP

#include <string>
#include <memory>

#include "resource.hpp"
#include "opengl_logger.hpp"

namespace djah { namespace opengl {

	template<int ShaderType>
	class shader_base
		: public resource
	{
	public:

		explicit shader_base(const std::string &source = "", const std::string &_fileName = "");
		virtual ~shader_base();

		const std::string &fileName() const;
		void loadSource(const std::string &source);
		bool compile() const;

	protected:

		virtual void aquire();
		virtual void release();
		virtual bool isValidResource() const;

		void loadSource() const;
		bool handleCompilationErrors() const;

		std::string source_;
		std::string fileName_;
	};

} /*opengl*/ } /*djah*/

#include "shader_base.inl"

#endif /* DJAH_OPENGL_SHADER_BASE_HPP */
