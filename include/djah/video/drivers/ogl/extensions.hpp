#ifndef DJAH_VIDEO_DRIVERS_OGL_EXTENSIONS_HPP
#define DJAH_VIDEO_DRIVERS_OGL_EXTENSIONS_HPP

//--------------------------------------------------------------------------------------------------
#ifdef DEFINE_EXTENSIONS_AS_LOCAL
#	define EXTERN
#else
#	define EXTERN extern
#endif

#define DEFINE_EXTENSION(PFN, EXT) EXTERN PFN EXT
//--------------------------------------------------------------------------------------------------

namespace djah { namespace video { namespace drivers { namespace ogl {

	void load_extensions();
	
	//----------------------------------------------------------------------------------------------
	// BUFFERS
	//----------------------------------------------------------------------------------------------
	DEFINE_EXTENSION( PFNGLGENVERTEXARRAYSPROC,			glGenVertexArrays			);
	DEFINE_EXTENSION( PFNGLDELETEVERTEXARRAYSPROC,		glDeleteVertexArrays		);
	DEFINE_EXTENSION( PFNGLISVERTEXARRAYPROC,			glIsVertexArray				);
	DEFINE_EXTENSION( PFNGLBINDVERTEXARRAYPROC,			glBindVertexArray			);

	DEFINE_EXTENSION( PFNGLENABLEVERTEXATTRIBARRAYPROC,	glEnableVertexAttribArray	);
	DEFINE_EXTENSION( PFNGLDISABLEVERTEXATTRIBARRAYPROC,glDisableVertexAttribArray	);
	DEFINE_EXTENSION( PFNGLVERTEXATTRIBPOINTERPROC,		glVertexAttribPointer		);

	DEFINE_EXTENSION( PFNGLGENRENDERBUFFERSPROC,		glGenRenderbuffers			);
	DEFINE_EXTENSION( PFNGLDELETERENDERBUFFERSPROC,		glDeleteRenderbuffers		);
	DEFINE_EXTENSION( PFNGLISRENDERBUFFERPROC,			glIsRenderbuffer			);
	DEFINE_EXTENSION( PFNGLBINDRENDERBUFFERPROC,		glBindRenderbuffer			);
	DEFINE_EXTENSION( PFNGLRENDERBUFFERSTORAGEPROC,		glRenderbufferStorage		);

	DEFINE_EXTENSION( PFNGLGENFRAMEBUFFERSPROC,			glGenFramebuffers			);
	DEFINE_EXTENSION( PFNGLDELETEFRAMEBUFFERSPROC,		glDeleteFramebuffers	);
	DEFINE_EXTENSION( PFNGLISFRAMEBUFFERPROC,			glIsFramebuffer				);
	DEFINE_EXTENSION( PFNGLBINDFRAMEBUFFERPROC,			glBindFramebuffer			);
	DEFINE_EXTENSION( PFNGLFRAMEBUFFERRENDERBUFFERPROC,	glFramebufferRenderbuffer	);

	//----------------------------------------------------------------------------------------------
	// Shaders
	//----------------------------------------------------------------------------------------------
	DEFINE_EXTENSION( PFNGLCREATEPROGRAMPROC,			glCreateProgram				);
	DEFINE_EXTENSION( PFNGLDELETEPROGRAMPROC,			glDeleteProgram				);
	DEFINE_EXTENSION( PFNGLISPROGRAMPROC,				glIsProgram					);
	DEFINE_EXTENSION( PFNGLLINKPROGRAMPROC,				glLinkProgram				);
	DEFINE_EXTENSION( PFNGLUSEPROGRAMPROC,				glUseProgram				);
	DEFINE_EXTENSION( PFNGLGETATTRIBLOCATIONPROC,		glGetAttribLocation			);
	DEFINE_EXTENSION( PFNGLGETPROGRAMIVPROC,			glGetProgramiv				);
	DEFINE_EXTENSION( PFNGLGETPROGRAMINFOLOGPROC,		glGetProgramInfoLog			);
	DEFINE_EXTENSION( PFNGLGETUNIFORMLOCATIONPROC,		glGetUniformLocation		);

	//----------------------------------------------------------------------------------------------
	// Uniforms
	//----------------------------------------------------------------------------------------------
	DEFINE_EXTENSION( PFNGLUNIFORM1FPROC,				glUniform1f					);
	DEFINE_EXTENSION( PFNGLUNIFORM2FPROC,				glUniform2f					);
	DEFINE_EXTENSION( PFNGLUNIFORM3FPROC,				glUniform3f					);
	DEFINE_EXTENSION( PFNGLUNIFORM4FPROC,				glUniform4f					);

	DEFINE_EXTENSION( PFNGLUNIFORM1FVPROC,				glUniform1fv				);
	DEFINE_EXTENSION( PFNGLUNIFORM2FVPROC,				glUniform2fv				);
	DEFINE_EXTENSION( PFNGLUNIFORM3FVPROC,				glUniform3fv				);
	DEFINE_EXTENSION( PFNGLUNIFORM4FVPROC,				glUniform4fv				);
	
	DEFINE_EXTENSION( PFNGLUNIFORM1IPROC,				glUniform1i					);
	DEFINE_EXTENSION( PFNGLUNIFORM2IPROC,				glUniform2i					);
	DEFINE_EXTENSION( PFNGLUNIFORM3IPROC,				glUniform3i					);
	DEFINE_EXTENSION( PFNGLUNIFORM4IPROC,				glUniform4i					);

	DEFINE_EXTENSION( PFNGLUNIFORM1IVPROC,				glUniform1iv				);
	DEFINE_EXTENSION( PFNGLUNIFORM2IVPROC,				glUniform2iv				);
	DEFINE_EXTENSION( PFNGLUNIFORM3IVPROC,				glUniform3iv				);
	DEFINE_EXTENSION( PFNGLUNIFORM4IVPROC,				glUniform4iv				);

} /*ogl*/ } /*drivers*/ } /*video*/ } /*djah*/

//--------------------------------------------------------------------------------------------------
#undef DEFINE_EXTENSION
//--------------------------------------------------------------------------------------------------

#endif /* DJAH_VIDEO_DRIVERS_OGL_EXTENSIONS_HPP */