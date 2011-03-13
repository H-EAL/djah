#include "application.hpp"

#include <djah/video/projection.hpp>
#include <djah/video/ogl.hpp>
#include <djah/video/text.hpp>
#include <djah/video/font_engine.hpp>

#include <djah/system/video_config.hpp>
#include <djah/system/device_base.hpp>
#include <djah/system/driver_base.hpp>

#include <djah/log/logger.hpp>
#include <djah/log/console_logger.hpp>

#include <djah/time/clock.hpp>

#include <djah/filesystem/browser.hpp>
#include <djah/filesystem/directory_source.hpp>
#include <djah/filesystem/memory_stream.hpp>

#include <djah/resources/media_manager.hpp>
#include <djah/resources/resource_manager.hpp>
#include <djah/resources/image_loader.hpp>

using namespace djah;

template<typename T>
boost::shared_ptr<T> find_resource(const std::string &url)
{
	static resources::default_media_manager s_dmm;
	static bool initialized = false;
	if(!initialized)
	{
		s_dmm.registerLoader( new resources::image_loader, "png tga jpg");
		initialized = true;
	}

	// Get it from the resource manager
	boost::shared_ptr<T> res = resources::resource_manager::get().find<T>(url);
	// If not found try to load it through the media manager
	if( !res && (res = boost::shared_ptr<T>(s_dmm.loadFromUrl<T>(url))) )
		resources::resource_manager::get().add(url, res);
	else if( !res )
		DJAH_BEGIN_LOG(EWL_CRITICAL) << "[ResourceManager] Unable to find the following resource: " << url << DJAH_END_LOG();

	return res;
}

struct mesh
{
	mesh(const std::string &name, const std::string textureName = "")
		: mat_world_(math::matrix4f::mat_identity)
		, vb_(0)
		, va_(0)
		, tex_(0)
	{
		strm_ = new filesystem::memory_stream( filesystem::browser::get().openReadStream("3d/" + name + ".bdae") );
		if( strm_->size() > 0 )
			init(textureName);
	}

	~mesh()
	{
		delete strm_;
		delete va_;
		delete vb_;
		delete tex_;
	}

	void init(const std::string &textureName)
	{
		if( !textureName.empty() )
		{
			boost::shared_ptr<resources::image> img = find_resource<resources::image>("textures/" + textureName);
			if( img )
			{
				tex_ = new video::ogl::texture(img->width(), img->height());
				tex_->setPixelBuffer(img->pixels(), false);
			}
		}

		video::ogl::vertex_format vf;
		vf.record()
			<< video::ogl::format::position<3,float>()
			<< video::ogl::format::normal<3,float>()
			<< video::ogl::format::tex_coord<2,float>();
		
		vb_ = new video::ogl::vertex_buffer( strm_->size(), video::ogl::EBU_STATIC_DRAW);
		va_ = new video::ogl::vertex_array(vf, vb_);
		vb_->write(strm_->buffer(), strm_->size());

		video::ogl::vertex_shader vs("shaders/test.vert");
		video::ogl::pixel_shader ps(tex_ ? "shaders/test.frag" : "shaders/lit_no_texture.frag");

		vs.compile();
		ps.compile();
		sp_.attach(vs);
		sp_.attach(ps);
		sp_.link();

		va_->init(sp_);
	}

	void draw(const math::matrix4f &matProj, const math::matrix4f &matView)
	{
		sp_.begin();
		if( tex_ )
		{
			glEnable(GL_TEXTURE_2D);
			tex_->bind();
			sp_.sendUniform("tex", 0);
		}
		else
		{
			sp_.sendUniform<4>("in_Color", math::vector4f(0.2f,1.0f,0.35f,1.0f).data);
		}
		sp_.sendUniformMatrix("Projection", (matProj), true);
		sp_.sendUniformMatrix("ModelView", (matView*mat_world_), true);
		va_->draw();
		sp_.end();
		glDisable(GL_TEXTURE_2D);
	}

	filesystem::memory_stream *strm_;
	video::ogl::vertex_buffer *vb_;
	video::ogl::vertex_array *va_;
	video::ogl::shader_program sp_;
	video::ogl::texture *tex_;
	math::matrix4f mat_world_;
};


application::application(int w, int h)
	: application_base(djah::system::video_config(w,h,32,24,0,false,true))
	, eye_(0,21.8f,math::pi_over_2)
	, center_(0,10,0)
	, up_(0,1,0)
{
}

void application::initImpl()
{
	log::logger::setLogger(new log::console_logger);
	DJAH_BEGIN_LOG(EWL_NOTIFICATION) << "application::initImpl()" << DJAH_END_LOG();
		
	filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("."));
	filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("./data"));

	const float w = static_cast<float>(device_->videoConfig().width);
	const float h = static_cast<float>(device_->videoConfig().height);
	matPerspectiveProj_ = video::make_perspective_projection(60.0f, w/h, 0.1f, 1000.f);
	matOrthoProj_ = video::make_orthographic_projection(0.0f, w, h, 0.0f, -1.0f, 1.0f);
		
	glClearDepth(1.f);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	video::font_engine::create();
	video::font_engine::get().setFontsPath("fonts/");

	cthulhu_ = new mesh("cthulhu");
	astroboy_ = new mesh("astroboy", "boy_10.tga");
}

void application::runImpl()
{
	static int fps = 0;
	static time::clock clk;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/**/
	static float angle = 0.0f;
	eye_.x = 20*cos(angle);
	eye_.z = 20*sin(angle);
	const float da = 0.0075f;
	angle = (angle+da > math::pi_times_2) ? 0.0f : angle+da;
	matView_ = video::make_look_at(eye_, center_, up_);

	draw3D();
	/**/
	draw2D();

	if( clk.getElapsedTimeMs() < 1000 )
		++fps;
	else
	{
		std::stringstream ss;
		ss << fps+1 << " FPS";
		fps_str_ = ss.str();
		clk.restart();
		fps = 0;
	}
}

void application::exitImpl()
{
	video::font_engine::destroy();

	delete cthulhu_;
	delete astroboy_;
}

void application::draw3D()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(matPerspectiveProj_.getTransposed().data);
		
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(matView_.getTransposed().data);

	drawAxis();
	drawMeshes();
}

void application::draw2D()
{
	const float w = static_cast<float>(device_->videoConfig().width);
	const float h = static_cast<float>(device_->videoConfig().height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(matOrthoProj_.getTransposed().data);
		
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	fps_str_.draw();
	/*
	glBegin(GL_TRIANGLE_STRIP);
	{
		static const float s = 50.0f;
		glColor4f(0.9f, 0.6f, 0.2f, 1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(0,0);
		glTexCoord2f(1.0f*s, 0.0f);
		glVertex2f(w,0);

		glColor4f(0.9f, 0.6f, 0.25f, 0.5f);
		glTexCoord2f(0.0f, 0.5f*s);
		glVertex2f(0,h/2);
		glTexCoord2f(1.0f*s, 0.5f*s);
		glVertex2f(w,h/2);
	}
	glEnd();
	*/
}

void application::drawAxis()
{
	glBegin(GL_LINES);
	{
		//X
		glColor3fv(math::vector3f::x_axis.data);
		glVertex3fv(math::vector3f::null_vector.data);
		glVertex3fv(math::vector3f::x_axis.data);
		//Y
		glColor3fv(math::vector3f::y_axis.data);
		glVertex3fv(math::vector3f::null_vector.data);
		glVertex3fv(math::vector3f::y_axis.data);
		//Z
		glColor3fv(math::vector3f::z_axis.data);
		glVertex3fv(math::vector3f::null_vector.data);
		glVertex3fv(math::vector3f::z_axis.data);
	}
	glEnd();

	static const float s = 50.0f;
	static const float c = 0.8f;
	static const float h = -0.1f;
	glScalef(s, 1.0f, s);
	glBegin(GL_QUADS);
	{
		glColor3f(c,c,c);
		glVertex3f(-0.5f, h, -0.5f);
		glVertex3f(-0.5f, h,  0.5f);
		glVertex3f( 0.5f, h,  0.5f);
		glVertex3f( 0.5f, h, -0.5f);
	}
	glEnd();
}

void application::drawMeshes()
{
	static const math::matrix4f rotA = 
		math::make_rotation(math::deg_to_rad(90.0f), 0.0f, 0.0f, 1.0f) *
		math::make_rotation(math::deg_to_rad(90.0f), 0.0f, 1.0f, 0.0f) *
		math::make_rotation(math::deg_to_rad(90.0f), 0.0f, 0.0f, 1.0f) *
		math::make_scale(0.2f, 0.2f, 0.2f);

	static const math::matrix4f rotC =
		math::make_translation(0.0f, 0.0f, -1.0f) *
		math::make_rotation(math::deg_to_rad(90.0f), 0.0f, 0.0f, 1.0f) *
		math::make_rotation(math::deg_to_rad(90.0f), 0.0f, 1.0f, 0.0f) *
		math::make_scale(2.0f, 2.0f, 2.0f);

	astroboy_->mat_world_ = math::make_translation(0.0f, 0.0f, 5.0f) * rotA;
	astroboy_->draw(matPerspectiveProj_, matView_);

	cthulhu_->mat_world_ = math::make_translation(0.0f, 2.8f, 0.0f) * rotC;
	cthulhu_->draw(matPerspectiveProj_, matView_);
}