#include "djah/debug/assertion.hpp"
#include "djah/platform.hpp"

#define DEFINE_EXTENSIONS_AS_LOCAL
#	include "djah/system/gl.hpp"
#undef DEFINE_EXTENSIONS_AS_LOCAL

namespace {
	
	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline void load_extension(T& func, const char *name)
	{
        #if defined(DJAH_COMPILE_LINUX)
			func = reinterpret_cast<T>( glXGetProcAddress(reinterpret_cast<const GLubyte *>(name)) );
        #elif defined(DJAH_COMPILE_WINDOWS)
			func = reinterpret_cast<T>( wglGetProcAddress(name) );
        #endif
		DJAH_ASSERT_MSG(func != nullptr, "Unable to load extension");
	}
	//----------------------------------------------------------------------------------------------

}

//--------------------------------------------------------------------------------------------------
#define LOAD_EXTENSION(EXT) load_extension(EXT, #EXT)
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
namespace djah { namespace system {

	void load_extensions()
	{
		//------------------------------------------------------------------------------------------
		// Buffers
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
		//LOAD_EXTENSION( glBindVertexBuffer			);

		LOAD_EXTENSION( glEnableVertexAttribArray	);
		LOAD_EXTENSION( glDisableVertexAttribArray	);
		LOAD_EXTENSION( glVertexAttribPointer		);
		LOAD_EXTENSION( glVertexAttribDivisor		);
		//LOAD_EXTENSION( glVertexAttribFormat		);
		//LOAD_EXTENSION( glVertexAttribBinding		);

		LOAD_EXTENSION( glGenRenderbuffers			);
		LOAD_EXTENSION( glDeleteRenderbuffers		);
		LOAD_EXTENSION( glIsRenderbuffer			);
		LOAD_EXTENSION( glBindRenderbuffer			);
		LOAD_EXTENSION( glRenderbufferStorage		);

		LOAD_EXTENSION( glGenFramebuffers			);
		LOAD_EXTENSION( glDeleteFramebuffers		);
		LOAD_EXTENSION( glIsFramebuffer				);
		LOAD_EXTENSION( glBindFramebuffer			);
		LOAD_EXTENSION( glCheckFramebufferStatus	);
		LOAD_EXTENSION( glFramebufferRenderbuffer	);
		LOAD_EXTENSION( glFramebufferTexture		);
		LOAD_EXTENSION( glDrawBuffers				);

		LOAD_EXTENSION( glDrawArraysInstanced		);

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
		LOAD_EXTENSION( glGetActiveUniform	 );
		LOAD_EXTENSION( glGetActiveAttrib	 );
		LOAD_EXTENSION( glPatchParameteri	 );

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

		LOAD_EXTENSION( glUniformMatrix2fv		);
		LOAD_EXTENSION( glUniformMatrix3fv		);
		LOAD_EXTENSION( glUniformMatrix4fv		);
		LOAD_EXTENSION( glUniformMatrix2x3fv	);
		LOAD_EXTENSION( glUniformMatrix3x2fv	);
		LOAD_EXTENSION( glUniformMatrix2x4fv	);
		LOAD_EXTENSION( glUniformMatrix4x2fv	);
		LOAD_EXTENSION( glUniformMatrix3x4fv	);
		LOAD_EXTENSION( glUniformMatrix4x3fv	);

		//------------------------------------------------------------------------------------------
		// Textures
		//------------------------------------------------------------------------------------------
		LOAD_EXTENSION( glActiveTexture		);
		LOAD_EXTENSION( glGenerateMipmap	);

		//------------------------------------------------------------------------------------------
		// Debug
		//------------------------------------------------------------------------------------------
		LOAD_EXTENSION( glDebugMessageCallbackARB );

		//------------------------------------------------------------------------------------------
		// Other
		//------------------------------------------------------------------------------------------
		LOAD_EXTENSION( glBlendEquation );
		LOAD_EXTENSION( glGetStringi	);
	}

} /*system*/ } /*djah*/
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
#undef LOAD_EXTENSION
//--------------------------------------------------------------------------------------------------