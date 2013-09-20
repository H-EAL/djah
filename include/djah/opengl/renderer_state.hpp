#ifndef DJAH_OPENGL_RENDERER_STATE_HPP
#define DJAH_OPENGL_RENDERER_STATE_HPP

namespace djah { namespace opengl {
	
	template<GLenum Enum>
	struct render_state
	{
		render_state()
		{
			glEnable(Enum);
		}

		~render_state()
		{
			glDisable(Enum);
		}

		static bool is_enabled()
		{
			return glIsEnabled(Enum);
		}
	};

	template<typename StatesList>
	class renderer_state;

	renderer_state
	<TYPELIST(
		GL_BLEND,
		GL_CULL_FACE,
		GL_DEPTH_TEST,
		GL_LINE_SMOOTH
	)> rendererState;


} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_RENDERER_STATE_HPP */