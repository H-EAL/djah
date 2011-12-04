#ifndef DJAH_VIDEO_OGL_SHADER_PROGRAM_HPP
#define DJAH_VIDEO_OGL_SHADER_PROGRAM_HPP

#include <map>
#include <string>

#include "../../../math/matrix.hpp"
#include "../../../system/opengl_include.hpp"
#include "shader_base.hpp"

namespace djah { namespace video { namespace ogl {

	class shader_program
		: public resource
	{
	public:

		shader_program();
		virtual ~shader_program();

		template<int ShaderType>
		void attach(const shader_base<ShaderType> &s) const;
		template<int ShaderType>
		void detach(const shader_base<ShaderType> &s) const;

		void link() const;

		void begin() const;
		static void end();

		unsigned int getVertexAttributeLocation(const std::string &name) const;

		void sendUniform(const std::string &name, float u1) const;
		void sendUniform(const std::string &name, float u1, float u2) const;
		void sendUniform(const std::string &name, float u1, float u2, float u3) const;
		void sendUniform(const std::string &name, float u1, float u2, float u3, float u4) const;

		void sendUniform(const std::string &name, int u1) const;
		void sendUniform(const std::string &name, int u1, int u2) const;
		void sendUniform(const std::string &name, int u1, int u2, int u3) const;
		void sendUniform(const std::string &name, int u1, int u2, int u3, int u4) const;

		template<int N, typename T>
		void sendUniform(const std::string &name, const T *data, int count = 1) const;
		template<int N, typename T>
		void sendUniform(const std::string &name, const math::vector<N,T> &vec) const;

		template<int N>
		void sendUniformMatrix(const std::string &name, const float *data, int count = 1, bool transpose = false) const;
		template<int N>
		void sendUniformMatrix(const std::string &name, const math::matrix<N,float> &mat, bool transpose = false) const;


	private:

		virtual void aquire();
		virtual void release();
		virtual bool isValidResource() const;

		void handleLinkingErrors() const;

		unsigned int getUniformLocation(const std::string &name) const;

		typedef std::map<std::string, unsigned int> uniform_cache_t;

		mutable uniform_cache_t cache_;
	};

} /*ogl*/ } /*video*/ } /*djah*/

#include "shader_program.inl"

#endif /* DJAH_VIDEO_OGL_SHADER_PROGRAM_HPP */
