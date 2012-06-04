#ifndef MODEL_HPP
#define MODEL_HPP

#include <memory>
#include "profiler.hpp"
#include "resource_finder.hpp"

struct mesh
{
	mesh(const std::string &name, const std::string textureName = "", const std::string &nmap = "", int tc = 2)
		: mat_world_(math::matrix4f::mat_identity)
		, vb_(0)
		, va_(0)
		, tex_(0)
		, nmap_(0)
	{
		strm_ = new filesystem::memory_stream( filesystem::browser::get().openReadStream("3d/" + name + ".bdae") );
		if( strm_->size() > 0 )
			init(textureName, nmap, tc);
	}

	~mesh()
	{
		delete strm_;
		delete va_;
		delete vb_;
		delete tex_;
		delete nmap_;
	}

	std::string loadShaderSource(const std::string &url)
	{
		std::string source;

		filesystem::stream_ptr strm = filesystem::browser::get().openReadStream(url);
		if( strm )
		{
			unsigned int src_size = strm->size();
			source.resize(src_size);
			strm->read(&source[0], src_size);
		}

		return source;
	}

	void init(const std::string &textureName, const std::string &nmap, int tc)
	{
		DJAH_AUTO_PROFILE("INIT MESH " + textureName);
		if( !textureName.empty() )
		{
			std::shared_ptr<resources::image> img;
			{
				//DJAH_AUTO_PROFILE("LOAD TEXTURE " + textureName);
				img = find_resource<resources::image>("textures/" + textureName);
			}
			if( img )
			{
				//DJAH_AUTO_PROFILE("CREATING TEXTURE " + textureName);
				tex_ = new video::ogl::texture(img->width(), img->height());
				tex_->setPixelBuffer(img->pixels(), false);
			}
		}
		if( !nmap.empty() )
		{
			std::shared_ptr<resources::image> img = find_resource<resources::image>("textures/" + nmap);
			if( img )
			{
				nmap_ = new video::ogl::texture(img->width(), img->height());
				nmap_->setPixelBuffer(img->pixels(), false);
			}
		}

		video::ogl::vertex_format vf;

		if( tc == 2 )
		{
			vf.record()
				<< video::ogl::format::position<3,float>()
				<< video::ogl::format::normal<3,float>()
				<< video::ogl::format::tex_coord<2,float>();
		}
		else
		{
			vf.record()
				<< video::ogl::format::position<3,float>()
				<< video::ogl::format::normal<3,float>()
				<< video::ogl::format::tex_coord<3,float>();
		}
		
		{
			//DJAH_AUTO_PROFILE("VERTEX STUFF " + textureName);
			vb_ = new video::ogl::vertex_buffer( strm_->size(), video::ogl::EBU_STATIC_DRAW);
			va_ = new video::ogl::vertex_array(vf, vb_);
			vb_->write(strm_->buffer(), strm_->size());
		}


		{
		//DJAH_AUTO_PROFILE("SHADERS STUFF " + textureName);
		video::ogl::vertex_shader vs( loadShaderSource("shaders/test.vert") );
		video::ogl::pixel_shader  ps( loadShaderSource("shaders/test.frag") );

		vs.compile();
		ps.compile();
		sp_.attach(vs);
		sp_.attach(ps);
		sp_.link();

		va_->init(sp_);
		}
	}

	void draw(const math::matrix4f &matProj, const math::matrix4f &matView, const math::vector3f &lightPos)
	{
		sp_.begin();

		sp_.sendUniform("in_LightPos", lightPos);

		if( tex_ )
		{
			glEnable(GL_TEXTURE_2D);
			tex_->bind();
			sp_.sendUniform("tex", 0);
			sp_.sendUniform("hasTexture", 1);
		}
		else
		{
			sp_.sendUniform("in_Color", math::vector4f(0.2f,1.0f,0.35f,1.0f));
			sp_.sendUniform("hasTexture", 0);
		}
		if( nmap_ )
		{
			glEnable(GL_TEXTURE_2D);
			nmap_->bind(1);
			sp_.sendUniform("nmap", 1);
		}
		sp_.sendUniformMatrix("Projection", matProj);
		sp_.sendUniformMatrix("Model", mat_world_);
		sp_.sendUniformMatrix("View", matView);
		va_->draw();
		sp_.end();
		glDisable(GL_TEXTURE_2D);
	}

	filesystem::memory_stream *strm_;
	video::ogl::vertex_buffer *vb_;
	video::ogl::vertex_array *va_;
	video::ogl::shader_program sp_;
	video::ogl::texture *tex_;
	video::ogl::texture *nmap_;
	math::matrix4f mat_world_;
};

#endif /* MODEL_HPP */