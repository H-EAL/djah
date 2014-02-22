#include "djah/3d/shader.hpp"

#include "djah/3d/3d_logger.hpp"
#include "djah/debug/assertion.hpp"
#include "djah/filesystem/browser.hpp"

namespace djah { namespace d3d {

	//----------------------------------------------------------------------------------------------
	shader::shader(const std::string &shaderName)
		: vertexShader_  (loadSource("shaders/" + shaderName + ".vert"    ), shaderName + ".vert")
		, tessCtrlShader_(loadSource("shaders/" + shaderName + ".tessctrl"), shaderName + ".tessctrl")
		, tessEvalShader_(loadSource("shaders/" + shaderName + ".tesseval"), shaderName + ".tesseval")
		, geometryShader_(loadSource("shaders/" + shaderName + ".geom"    ), shaderName + ".geom")
		, fragmentShader_(loadSource("shaders/" + shaderName + ".frag"    ), shaderName + ".frag")
		, shaderProgram_(shaderName)
	{

		if( vertexShader_.compile() )
		{
			shaderProgram_.attach( vertexShader_ );
		}
		if( tessCtrlShader_.compile() )
		{
			shaderProgram_.attach( tessCtrlShader_ );
		}
		if( tessEvalShader_.compile() )
		{
			shaderProgram_.attach( tessEvalShader_ );
		}
		if( geometryShader_.compile() )
		{
			shaderProgram_.attach( geometryShader_ );
		}
		if( fragmentShader_.compile() )
		{
			shaderProgram_.attach( fragmentShader_ );
		}

		shaderProgram_.link();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	std::string shader::loadSource(const std::string &fileName) const
	{
		std::string source;

		djah::filesystem::stream_sptr pShaderSourceStrm = djah::filesystem::browser::get().openReadStream(fileName);
		if( pShaderSourceStrm )
		{
			const unsigned int src_size = pShaderSourceStrm->size();

			DJAH_ASSERT_MSG(src_size > 0, "Shader source is empty");
			
			if( src_size > 0 )
			{
				source.resize(src_size);
				pShaderSourceStrm->read(&source[0], src_size);
			}
		}

		return source;
	}
	//----------------------------------------------------------------------------------------------

} /*d3d*/ } /*djah*/