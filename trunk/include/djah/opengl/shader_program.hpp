#ifndef DJAH_OPENGL_SHADER_PROGRAM_HPP
#define DJAH_OPENGL_SHADER_PROGRAM_HPP

#include <map>
#include <string>
#include <memory>

#include "math/matrix.hpp"
#include "system/gl.hpp"
#include "shader_base.hpp"

namespace djah { namespace opengl {

	class shader_program
		: public resource
	{
	public:
		shader_program(const std::string &shaderName);
		virtual ~shader_program();

		const std::string& name() const { return name_; }

		template<int ShaderType>
		void attach(const shader_base<ShaderType> &s) const;
		template<int ShaderType>
		void detach(const shader_base<ShaderType> &s) const;

		bool link() const;

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

		template<int M, int N>
		void sendUniformMatrix(const std::string &name, const float *data, int count = 1, bool transpose = false) const;
		template<int M, int N>
		void sendUniformMatrix(const std::string &name, const math::matrix<M,N,float> &mat, bool transpose = false) const;


	private:
		virtual void aquire();
		virtual void release();
		virtual bool isValidResource() const;

		bool handleLinkingErrors() const;
		unsigned int getUniformLocation(const std::string &name) const;

	private:
		typedef std::map<std::string, unsigned int> uniform_cache_t;

		mutable uniform_cache_t cache_;
		std::string				name_;
	};

} /*opengl*/ } /*djah*/

#include "shader_program.inl"

#endif /* DJAH_OPENGL_SHADER_PROGRAM_HPP */
