#ifndef DJAH_VIDEO_DRIVERS_OGL_SHADER_HPP
#define DJAH_VIDEO_DRIVERS_OGL_SHADER_HPP

#include <map>
#include <string>

#include "../../../../math/basic_matrix.hpp"

#include "shader_base.hpp"

namespace djah { namespace video { namespace drivers { namespace ogl {

	class shader
		: public resource
	{
	public:

		shader();
		~shader();

		template<int ShaderType>
		void attach(const shader_base<ShaderType> &s) const;
		template<int ShaderType>
		void detach(const shader_base<ShaderType> &s) const;

		void link() const;

		void begin() const;
		void end()   const;

		unsigned int getVertexAttributeLocation(const std::string &name) const;

		void sendUniform(const std::string &name, float u1) const;
		void sendUniform(const std::string &name, float u1, float u2) const;
		void sendUniform(const std::string &name, float u1, float u2, float u3) const;
		void sendUniform(const std::string &name, float u1, float u2, float u3, float u4) const;

		void sendUniform(const std::string &name, int u1) const;
		void sendUniform(const std::string &name, int u1, int u2) const;
		void sendUniform(const std::string &name, int u1, int u2, int u3) const;
		void sendUniform(const std::string &name, int u1, int u2, int u3, int u4) const;

		template<size_t SIZE, typename T>
		void sendUniform(const std::string &name, const T *data, int count = 1) const;

		template<size_t SIZE>
		void sendUniformMatrix(const std::string &name, const float *data, int count = 1, bool transpose = false) const;
		template<size_t SIZE>
		void sendUniformMatrix(const std::string &name, const math::basic_matrix<SIZE,float> &mat, bool transpose = false) const;


	private:

		void aquire();
		void release();

		void handleLinkingErrors() const;

		unsigned int getUniformLocation(const std::string &name) const;

		typedef std::map<std::string, unsigned int> uniform_cache_t;

		mutable uniform_cache_t cache_;
	};

} /*ogl*/ } /*drivers*/ } /*video*/ } /*djah*/

#include "shader.inl"

#endif /* DJAH_VIDEO_DRIVERS_OGL_SHADER_HPP */