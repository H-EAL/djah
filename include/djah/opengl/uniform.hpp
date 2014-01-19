#ifndef DJAH_OPENGL_UNIFORM_HPP
#define DJAH_OPENGL_UNIFORM_HPP

#include "djah/system/gl.hpp"
#include "djah/opengl/shader_program.hpp"

namespace djah { namespace opengl {

	typedef std::shared_ptr<shader_program> shader_program_sptr;
	typedef std::weak_ptr<shader_program>   shader_program_wptr;

	template<typename T>
	class uniform
	{
	public:
		void send()
		{
			if( ensure(shader_program_sptr spShader = wpShader_.lock()) )
			{
				spShader->sendUniform(name_, value_);
			}
		}

		void sync()
		{
			if( ensure(shader_program_sptr spShader = wpShader_.lock()) )
			{
				value_ = spShader->getUniform<T>(name_);
			}
		}

		operator T() { return value_; }

	private:
		uniform(const shader_program_sptr &spShader,
			const std::string &name, int location)
			: wpShader_(spShader)
			, name_(name)
			, location_(location)
		{
		}

		~uniform() {}

	private:
		shader_program_wptr wpShader_;
		std::string name_;
		int location_;
		T value_;
	};

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_UNIFORM_HPP */
