#include "djah/debug/assertion.hpp"
#include "djah/platform.hpp"

#define DEFINE_EXTENSIONS_AS_LOCAL
#	include "djah/system/gl.hpp"
#undef DEFINE_EXTENSIONS_AS_LOCAL

namespace {
	
	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline void load_extension(T& func, const char *name, int major, int minor)
	{
		#if defined(DJAH_COMPILE_LINUX)
			func = reinterpret_cast<T>( glXGetProcAddress(reinterpret_cast<const GLubyte *>(name)) );
		#elif defined(DJAH_COMPILE_WINDOWS)
			func = reinterpret_cast<T>( wglGetProcAddress(name) );
		#endif
			DJAH_ASSERT_MSG(func != nullptr, "Unable to load extension \"%s\" (core since version %d.%d)", name, major, minor);
	}
	//----------------------------------------------------------------------------------------------

}

//--------------------------------------------------------------------------------------------------
#define LOAD_EXTENSION(MAJOR, MINOR, EXT) load_extension(EXT, #EXT, MAJOR, MINOR)
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
namespace djah { namespace system {

	void load_extensions()
	{
		//------------------------------------------------------------------------------------------
		// Buffers
		//------------------------------------------------------------------------------------------
		LOAD_EXTENSION( 3,0, glGenVertexArrays			);
		LOAD_EXTENSION( 3,0, glDeleteVertexArrays		);
		LOAD_EXTENSION( 3,0, glIsVertexArray			);
		LOAD_EXTENSION( 3,0, glBindVertexArray			);
		
		LOAD_EXTENSION( 1,5, glGenBuffers				);
		LOAD_EXTENSION( 1,5, glDeleteBuffers			);
		LOAD_EXTENSION( 1,5, glIsBuffer					);
		LOAD_EXTENSION( 1,5, glBindBuffer				);
		LOAD_EXTENSION( 1,5, glBufferData				);
		LOAD_EXTENSION( 1,5, glBufferSubData			);
		LOAD_EXTENSION( 1,5, glGetBufferSubData			);
		LOAD_EXTENSION( 4,3, glBindVertexBuffer			);
		LOAD_EXTENSION( 4,4, glBufferStorage			);

		LOAD_EXTENSION( 2,0, glEnableVertexAttribArray	);
		LOAD_EXTENSION( 2,0, glDisableVertexAttribArray	);
		LOAD_EXTENSION( 2,0, glVertexAttribPointer		);
		LOAD_EXTENSION( 2,0, glVertexAttribDivisor		);
		LOAD_EXTENSION( 4,3, glVertexAttribFormat		);
		LOAD_EXTENSION( 4,3, glVertexAttribBinding		);

		LOAD_EXTENSION( 3,0, glGenRenderbuffers			);
		LOAD_EXTENSION( 3,0, glDeleteRenderbuffers		);
		LOAD_EXTENSION( 3,0, glIsRenderbuffer			);
		LOAD_EXTENSION( 3,0, glBindRenderbuffer			);
		LOAD_EXTENSION( 3,0, glRenderbufferStorage		);

		LOAD_EXTENSION( 3,0, glGenFramebuffers			);
		LOAD_EXTENSION( 3,0, glDeleteFramebuffers		);
		LOAD_EXTENSION( 3,0, glIsFramebuffer			);
		LOAD_EXTENSION( 3,0, glBindFramebuffer			);
		LOAD_EXTENSION( 3,0, glCheckFramebufferStatus	);
		LOAD_EXTENSION( 3,0, glFramebufferRenderbuffer	);
		LOAD_EXTENSION( 3,2, glFramebufferTexture		);
		LOAD_EXTENSION( 2,0, glDrawBuffers				);
		LOAD_EXTENSION( 3,0, glBlitFramebuffer			);

		LOAD_EXTENSION( 3,1, glDrawArraysInstanced		);
		LOAD_EXTENSION( 3,1, glDrawElementsInstanced	);

		//------------------------------------------------------------------------------------------
		// Shaders
		//------------------------------------------------------------------------------------------
		LOAD_EXTENSION( 2,0, glCreateShader			);
		LOAD_EXTENSION( 2,0, glDeleteShader			);
		LOAD_EXTENSION( 2,0, glIsShader				);
		LOAD_EXTENSION( 2,0, glShaderSource			);
		LOAD_EXTENSION( 2,0, glGetShaderiv			);
		LOAD_EXTENSION( 2,0, glGetShaderInfoLog		);
		LOAD_EXTENSION( 2,0, glCompileShader		);
		LOAD_EXTENSION( 2,0, glCreateProgram		);
		LOAD_EXTENSION( 2,0, glDeleteProgram		);
		LOAD_EXTENSION( 2,0, glIsProgram			);
		LOAD_EXTENSION( 2,0, glAttachShader			);
		LOAD_EXTENSION( 2,0, glDetachShader			);
		LOAD_EXTENSION( 2,0, glLinkProgram			);
		LOAD_EXTENSION( 2,0, glUseProgram			);
		LOAD_EXTENSION( 2,0, glGetAttribLocation	);
		LOAD_EXTENSION( 2,0, glGetProgramiv			);
		LOAD_EXTENSION( 2,0, glGetProgramInfoLog	);
		LOAD_EXTENSION( 2,0, glGetUniformLocation	);
		LOAD_EXTENSION( 2,0, glGetActiveUniform		);
		LOAD_EXTENSION( 2,0, glGetActiveAttrib		);
		LOAD_EXTENSION( 4,0, glPatchParameteri		);

		//------------------------------------------------------------------------------------------
		// Uniforms
		//------------------------------------------------------------------------------------------
		LOAD_EXTENSION( 2,0, glUniform1f );
		LOAD_EXTENSION( 2,0, glUniform2f );
		LOAD_EXTENSION( 2,0, glUniform3f );
		LOAD_EXTENSION( 2,0, glUniform4f );
									 
		LOAD_EXTENSION( 2,0, glUniform1fv );
		LOAD_EXTENSION( 2,0, glUniform2fv );
		LOAD_EXTENSION( 2,0, glUniform3fv );
		LOAD_EXTENSION( 2,0, glUniform4fv );

		LOAD_EXTENSION( 2,0, glUniform1i );
		LOAD_EXTENSION( 2,0, glUniform2i );
		LOAD_EXTENSION( 2,0, glUniform3i );
		LOAD_EXTENSION( 2,0, glUniform4i );

		LOAD_EXTENSION( 2,0, glUniform1iv );
		LOAD_EXTENSION( 2,0, glUniform2iv );
		LOAD_EXTENSION( 2,0, glUniform3iv );
		LOAD_EXTENSION( 2,0, glUniform4iv );

		LOAD_EXTENSION( 2,0, glUniformMatrix2fv		);
		LOAD_EXTENSION( 2,0, glUniformMatrix3fv		);
		LOAD_EXTENSION( 2,0, glUniformMatrix4fv		);
		LOAD_EXTENSION( 2,0, glUniformMatrix2x3fv	);
		LOAD_EXTENSION( 2,0, glUniformMatrix3x2fv	);
		LOAD_EXTENSION( 2,0, glUniformMatrix2x4fv	);
		LOAD_EXTENSION( 2,0, glUniformMatrix4x2fv	);
		LOAD_EXTENSION( 2,0, glUniformMatrix3x4fv	);
		LOAD_EXTENSION( 2,0, glUniformMatrix4x3fv	);

		LOAD_EXTENSION( 2,0, glGetUniformfv		);
		LOAD_EXTENSION( 2,0, glGetUniformdv		);
		LOAD_EXTENSION( 2,0, glGetUniformiv		);
		LOAD_EXTENSION( 2,0, glGetUniformuiv	);

		//------------------------------------------------------------------------------------------
		// Textures
		//------------------------------------------------------------------------------------------
		LOAD_EXTENSION( 1,3, glActiveTexture		);
		LOAD_EXTENSION( 3,0, glGenerateMipmap		);
		LOAD_EXTENSION( 3,3, glGenSamplers			);
		LOAD_EXTENSION( 3,3, glDeleteSamplers		);
		LOAD_EXTENSION( 3,3, glBindSampler			);
		LOAD_EXTENSION( 3,3, glIsSampler			);
		LOAD_EXTENSION( 3,3, glSamplerParameteri	);
		LOAD_EXTENSION( 3,3, glSamplerParameterf	);

		//------------------------------------------------------------------------------------------
		// Debug
		//------------------------------------------------------------------------------------------
		LOAD_EXTENSION( 4,3, glDebugMessageCallback );

		//------------------------------------------------------------------------------------------
		// Other
		//------------------------------------------------------------------------------------------
		LOAD_EXTENSION( 2,0, glBlendEquation	);
		LOAD_EXTENSION( 1,0, glGetStringi		);
	}

} /*system*/ } /*djah*/
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
#undef LOAD_EXTENSION
//--------------------------------------------------------------------------------------------------