#ifndef MODEL_HPP
#define MODEL_HPP

#include <memory>
#include "profiler.hpp"
#include "resource_finder.hpp"
#include "djah/dataobject/global_registry.hpp"
#include "djah/opengl.hpp"
#include "djah/resources/media_manager.hpp"

struct model
{
	model(const std::string &doName)
		: mat_world_(djah::math::matrix4f::identity)
		, sp_(nullptr)
		, vb_(nullptr)
		, va_(nullptr)
		, tex_(nullptr)
		, nmap_(nullptr)
		, pDo_( djah::dataobject::default_registry::get().getDO(doName) )
	{
		assert( pDo_ );

		djah::filesystem::stream_ptr fstrm = djah::filesystem::browser::get().openReadStream(pDo_->get<std::string>("model_file"));

		if( fstrm && fstrm->size() > 0 )
		{
			djah::filesystem::memory_stream strm( fstrm );
			init
			(
				strm,
				pDo_->get<std::string>("texture_file", ""),
				pDo_->get<std::string>("nmap_file", ""),
				pDo_->get<int>("tex_coords", 2)
			);

			trans_ = djah::math::transformation_f
			(
				pDo_->get<djah::math::vector3f>("position"),
				pDo_->get<djah::math::quatf>("orientation"),
				pDo_->get<djah::math::vector3f>( "scale", math::vector3f(1.0f, 1.0f, 1.0f) )
			);
		}
	}

	~model()
	{
		delete sp_;
		delete vb_;
		delete va_;
		delete tex_;
		delete nmap_;
	}

	void init(djah::filesystem::memory_stream &strm, const std::string &textureName, const std::string &nmap, int tc)
	{
		if( !textureName.empty() )
		{
			djah::resources::image_ptr img = find_resource<resources::image>(textureName);
			if( img )
			{
				tex_ = new opengl::texture(GL_RGB, img->width(), img->height());
				tex_->bind();
				tex_->setBestFiltering();
				tex_->setPixelBuffer(GL_BGR, GL_UNSIGNED_BYTE, img->pixels());
			}
		}

		djah::opengl::vertex_format vf;

		if( tc == 2 )
		{
			vf.record()
				<< djah::opengl::format::position<3,float>()
				<< djah::opengl::format::normal<3,float>()
				<< djah::opengl::format::tex_coord<2,float>();
		}
		
		{
			vb_ = new djah::opengl::vertex_buffer(strm.size(), opengl::eBU_StaticDraw);
			va_ = new djah::opengl::vertex_array(vf, vb_);
			vb_->write(strm.buffer(), strm.size());
		}

		compileShader();
	}

	void compileShader()
	{
		if( sp_ )
		{
			delete sp_;
			sp_ = 0;
		}
		const std::string &vertexShaderFile = pDo_->get<std::string>("vertex_shader");
		const std::string &pixelShaderFile  = pDo_->get<std::string>("pixel_shader");
		
		djah::opengl::vertex_shader vs( loadShaderSource(vertexShaderFile) );
		djah::opengl::pixel_shader  ps( loadShaderSource(pixelShaderFile)  );

		sp_ = new djah::opengl::shader_program( filesystem::url(vertexShaderFile).baseName() );

		if( vs.compile() )
			sp_->attach(vs);

		if( ps.compile() )
			sp_->attach(ps);

		sp_->link();

		va_->init(*sp_);
	}

	//----------------------------------------------------------------------------------------------
	void draw(const djah::math::matrix4f &matProj, const djah::math::matrix4f &matView, const djah::math::matrix4f &lightVP, const djah::math::vector3f &spotPosition, const djah::math::vector3f &spotDirection)
	{
		sp_->begin();

		djah::opengl::texture::set_active_unit(0);
		tex_->bind();

		sp_->sendUniform("in_DiffuseSampler", 0);
		sp_->sendUniform("in_ShadowMapSampler", 1);
		sp_->sendUniform("in_SpotPosition", spotPosition);
		sp_->sendUniform("in_SpotDirection", spotDirection);

		mat_world_ = trans_.toMatrix4();
		sp_->sendUniformMatrix("in_WorldViewProjection", mat_world_ * matView * matProj);
		sp_->sendUniformMatrix("in_World", mat_world_);
		sp_->sendUniformMatrix("in_LightWVP", mat_world_ * lightVP);
		va_->draw();
		sp_->end();

		tex_->unbind();
	}
	//----------------------------------------------------------------------------------------------

	djah::opengl::shader_program *sp_;
	djah::opengl::vertex_buffer *vb_;
	djah::opengl::vertex_array *va_;
	djah::opengl::texture *tex_;
	djah::opengl::texture *nmap_;
	djah::math::matrix4f mat_world_;
	djah::math::transformation_f trans_;
	djah::dataobject::default_registry::data_object_ptr pDo_;
};

#endif /* MODEL_HPP */