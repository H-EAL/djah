#ifndef MODEL_HPP
#define MODEL_HPP

#include <memory>
#include "profiler.hpp"
#include "resource_finder.hpp"
#include "djah/dataobject/global_registry.hpp"
#include "djah/opengl.hpp"
#include "djah/resources/asset_finder.hpp"
#include "djah/3d/shader.hpp"

struct model
{
	model(const std::string &doName)
		: mat_world_(djah::math::matrix4f::identity)
		, sp_(nullptr)
		, vb_(nullptr)
		, tex_(nullptr)
		, nmap_(nullptr)
		, pDo_( djah::dataobject::default_registry::get().getDO(doName) )
	{
		DJAH_ASSERT( pDo_ );

		djah::filesystem::stream_sptr fstrm = djah::filesystem::browser::get().openReadStream(pDo_->get<std::string>("model_file"));

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

			trans_ = djah::math::transform_f
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
	}

	void init(djah::filesystem::memory_stream &strm, const std::string &textureName, const std::string &nmap, int tc)
	{
		if( !textureName.empty() )
		{
			tex_ = djah::d3d::texture_manager::get().find(textureName);
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
			vb_->write(strm.buffer(), strm.size());

			va_.addVertexBuffer(vb_,vf);
			va_.setVertexCount(vb_->size() / vf.vertexSize());
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
		const std::string &shaderFile = pDo_->get<std::string>("shader");
		sp_ = new djah::d3d::shader( shaderFile );
		va_.init(sp_->program());
	}

	//----------------------------------------------------------------------------------------------
	void draw(const djah::math::matrix4f &matProj, const djah::math::matrix4f &matView, const djah::math::matrix4f &lightVP, const djah::math::vector3f &spotPosition, const djah::math::vector3f &spotDirection)
	{
		sp_->program().begin();

		djah::opengl::texture::set_active_unit(0);
		tex_->bind();


		sp_->program().sendUniform("in_TexRepeat", 1.0f);
		sp_->program().sendUniform("in_DiffuseSampler", 0);
		sp_->program().sendUniform("in_ShadowMapSampler", 1);
		sp_->program().sendUniform("in_SpotPosition", spotPosition);
		sp_->program().sendUniform("in_SpotDirection", spotDirection);

		mat_world_ = trans_.toMatrix4();
		sp_->program().sendUniform("in_WorldViewProjection", mat_world_ * matView * matProj);
		sp_->program().sendUniform("in_World", mat_world_);
		sp_->program().sendUniform("in_LightWVP", mat_world_ * lightVP);
		va_.draw();
		sp_->program().end();

		tex_->unbind();
	}
	//----------------------------------------------------------------------------------------------

	djah::opengl::vertex_buffer *vb_;
	djah::opengl::vertex_array va_;
	djah::d3d::shader      *sp_;
	djah::d3d::texture_sptr tex_;
	djah::d3d::texture_sptr nmap_;
	djah::math::matrix4f mat_world_;
	djah::math::transform_f trans_;
	djah::dataobject::default_registry::data_object_ptr pDo_;
};

#endif /* MODEL_HPP */