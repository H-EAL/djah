#ifndef DJAH_OPENGL_SHADER_PROGRAM_HPP
#define DJAH_OPENGL_SHADER_PROGRAM_HPP

#include <map>
#include <string>
#include <memory>

#include "djah/system/gl.hpp"

#include "djah/math/vector.hpp"
#include "djah/math/matrix.hpp"
#include "djah/math/quaternion.hpp"

#include "shader_base.hpp"

namespace djah { namespace opengl {

	class shader_program
		: public resource
	{
		DJAH_OPENGL_RESOURCE(shader_program);

	public:
		typedef std::map<std::string, int> cache_t;

	public:
		shader_program(const std::string &shaderName);
		virtual ~shader_program();

		template<int ShaderType>
		void attach(const shader_base<ShaderType> &s) const;
		template<int ShaderType>
		void detach(const shader_base<ShaderType> &s) const;

		bool link();

		void begin() const;
		static void end();

		int  getVertexAttributeLocation(const std::string &name) const;
		const cache_t& getAttributeMap() const { return attributeCache_; }

		void sendUniform(const std::string &name, float u1) const;
		void sendUniform(const std::string &name, float u1, float u2) const;
		void sendUniform(const std::string &name, float u1, float u2, float u3) const;
		void sendUniform(const std::string &name, float u1, float u2, float u3, float u4) const;

		void sendUniform(const std::string &name, int u1) const;
		void sendUniform(const std::string &name, int u1, int u2) const;
		void sendUniform(const std::string &name, int u1, int u2, int u3) const;
		void sendUniform(const std::string &name, int u1, int u2, int u3, int u4) const;

		template<typename T>
		void sendUniform(const std::string &name, T val) const;
		template<typename T, int S>
		void sendUniform(const std::string &name, const T (&array)[S]) const;

		template<int N, typename T>
		void sendUniform(const std::string &name, const T *data, int count = 1) const;
		template<int N, typename T>
		void sendUniform(const std::string &name, const math::vector<N,T> &vec) const;
		template<int N, typename T, int S>
		void sendUniform(const std::string &name, const math::vector<N,T> (&vecArray)[S]) const;

		template<int M, int N>
		void sendUniform(const std::string &name, const float *data, int count = 1, bool transpose = false) const;
		template<int M, int N>
		void sendUniform(const std::string &name, const math::matrix<M,N,float> &mat, bool transpose = false) const;
		template<int M, int N, int S>
		void sendUniform(const std::string &name, const math::matrix<M,N,float> (&matArray)[S], bool transpose = false) const;

		template<typename T>
		T getUniform(const std::string &name) const;

	private:
		virtual void aquire();
		virtual void release();
		virtual bool isValidResource() const;

		bool handleLinkingErrors() const;
		void initializeUniformMap();
		void initializeAttributeMap();
		int  getUniformLocation(const std::string &name) const;

	private:
		cache_t uniformCache_;
		cache_t attributeCache_;
	};

} /*opengl*/ } /*djah*/

#include "shader_program.inl"

#endif /* DJAH_OPENGL_SHADER_PROGRAM_HPP */
