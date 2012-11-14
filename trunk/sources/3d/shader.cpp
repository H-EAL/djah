#include "djah/3d/shader.hpp"

#include "djah/3d/3d_logger.hpp"
#include "djah/debug/assertion.hpp"
#include "djah/filesystem/browser.hpp"

namespace djah { namespace d3d {

	//----------------------------------------------------------------------------------------------
	shader::shader(const std::string &shaderName)
		: vertexShader_(loadSource("shaders/" + shaderName + ".vert"), shaderName + ".vert")
		, pixelShader_ (loadSource("shaders/" + shaderName + ".frag"), shaderName + ".frag")
		, shaderProgram_(shaderName)
	{

		if( vertexShader_.compile() && pixelShader_.compile() )
		{
			shaderProgram_.attach( vertexShader_ );
			shaderProgram_.attach( pixelShader_ );
			shaderProgram_.link();
		}
		else
		{
			DJAH_3D_ERROR() << "Unable to compile shader: " << shaderName << DJAH_END_LOG();
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	std::string shader::loadSource(const std::string &fileName) const
	{
		std::string source;

		djah::filesystem::stream_ptr pShaderSourceStrm = djah::filesystem::browser::get().openReadStream(fileName);

		DJAH_ASSERT_MSG(pShaderSourceStrm != nullptr, "Unable to open shader file: %s", fileName.c_str());

		if( pShaderSourceStrm )
		{
			unsigned int src_size = pShaderSourceStrm->size();
			source.resize(src_size);
			pShaderSourceStrm->read(&source[0], src_size);
		}

		return source;
	}
	//----------------------------------------------------------------------------------------------

} /*d3d*/ } /*djah*/