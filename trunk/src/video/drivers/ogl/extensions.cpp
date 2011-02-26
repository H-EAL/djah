#include <cassert>

#define DEFINE_EXTENSIONS_AS_LOCAL
#include "video/drivers/opengl_include.hpp"
#undef DEFINE_EXTENSIONS_AS_LOCAL

namespace {
	
	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline void load_extension(T& func, const char *name)
	{
		func = reinterpret_cast<T>(wglGetProcAddress(name));
		assert(func != 0);
	}
	//----------------------------------------------------------------------------------------------

}

//--------------------------------------------------------------------------------------------------
#define LOAD_EXTENSION(EXT) load_extension(EXT, #EXT)
//--------------------------------------------------------------------------------------------------

namespace djah { namespace video { namespace drivers { namespace ogl {

	void load_extensions()
	{
		//------------------------------------------------------------------------------------------
		// BUFFERS
		//------------------------------------------------------------------------------------------
		LOAD_EXTENSION( glGenVertexArrays			);
		LOAD_EXTENSION( glDeleteVertexArrays		);
		LOAD_EXTENSION( glIsVertexArray				);
		LOAD_EXTENSION( glBindVertexArray			);
		
		LOAD_EXTENSION( glGenBuffers				);
		LOAD_EXTENSION( glDeleteBuffers				);
		LOAD_EXTENSION( glIsBuffer					);
		LOAD_EXTENSION( glBindBuffer				);
		LOAD_EXTENSION( glBufferData				);
		LOAD_EXTENSION( glBufferSubData				);

		LOAD_EXTENSION( glEnableVertexAttribArray	);
		LOAD_EXTENSION( glDisableVertexAttribArray	);
		LOAD_EXTENSION( glVertexAttribPointer		);

		LOAD_EXTENSION( glGenRenderbuffers			);
		LOAD_EXTENSION( glDeleteRenderbuffers		);
		LOAD_EXTENSION( glIsRenderbuffer			);
		LOAD_EXTENSION( glBindRenderbuffer			);
		LOAD_EXTENSION( glRenderbufferStorage		);

		LOAD_EXTENSION( glGenFramebuffers			);
		LOAD_EXTENSION( glDeleteFramebuffers		);
		LOAD_EXTENSION( glIsFramebuffer				);
		LOAD_EXTENSION( glBindFramebuffer			);
		LOAD_EXTENSION( glFramebufferRenderbuffer	);

		//------------------------------------------------------------------------------------------
		// Shaders
		//------------------------------------------------------------------------------------------
		LOAD_EXTENSION( glCreateShader		 );
		LOAD_EXTENSION( glDeleteShader		 );
		LOAD_EXTENSION( glIsShader			 );
		LOAD_EXTENSION( glShaderSource		 );
		LOAD_EXTENSION( glGetShaderiv		 );
		LOAD_EXTENSION( glGetShaderInfoLog	 );
		LOAD_EXTENSION( glCompileShader		 );
		LOAD_EXTENSION( glCreateProgram		 );
		LOAD_EXTENSION( glDeleteProgram		 );
		LOAD_EXTENSION( glIsProgram			 );
		LOAD_EXTENSION( glAttachShader		 );
		LOAD_EXTENSION( glDetachShader		 );
		LOAD_EXTENSION( glLinkProgram		 );
		LOAD_EXTENSION( glUseProgram		 );
		LOAD_EXTENSION( glGetAttribLocation	 );
		LOAD_EXTENSION( glGetProgramiv		 );
		LOAD_EXTENSION( glGetProgramInfoLog	 );
		LOAD_EXTENSION( glGetUniformLocation );

		//------------------------------------------------------------------------------------------
		// Uniforms
		//------------------------------------------------------------------------------------------
		LOAD_EXTENSION( glUniform1f	 );
		LOAD_EXTENSION( glUniform2f	 );
		LOAD_EXTENSION( glUniform3f	 );
		LOAD_EXTENSION( glUniform4f	 );
									 
		LOAD_EXTENSION( glUniform1fv );
		LOAD_EXTENSION( glUniform2fv );
		LOAD_EXTENSION( glUniform3fv );
		LOAD_EXTENSION( glUniform4fv );
									 
		LOAD_EXTENSION( glUniform1i	 );
		LOAD_EXTENSION( glUniform2i	 );
		LOAD_EXTENSION( glUniform3i	 );
		LOAD_EXTENSION( glUniform4i	 );
									 
		LOAD_EXTENSION( glUniform1iv );
		LOAD_EXTENSION( glUniform2iv );
		LOAD_EXTENSION( glUniform3iv );
		LOAD_EXTENSION( glUniform4iv );

		LOAD_EXTENSION( glUniformMatrix2fv );
		LOAD_EXTENSION( glUniformMatrix3fv );
		LOAD_EXTENSION( glUniformMatrix4fv );
	}

} /*ogl*/ } /*drivers*/ } /*video*/ } /*djah*/

//--------------------------------------------------------------------------------------------------
#undef LOAD_EXTENSION
//--------------------------------------------------------------------------------------------------