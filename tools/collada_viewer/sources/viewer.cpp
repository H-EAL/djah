#include "viewer.hpp"

#include <djah/application_base.hpp>
#include <djah/time/clock.hpp>
#include <djah/math.hpp>

#include <djah/system/video_config.hpp>
#include <djah/system/opengl_include.hpp>
#include <djah/system/device_base.hpp>

#include <djah/video/ogl.hpp>
#include <djah/video/projection.hpp>
#include <djah/video/font_engine.hpp>

#include <djah/resources/resource_manager.hpp>
#include <djah/resources/media_manager.hpp>
#include <djah/resources/image_loader.hpp>

#include <djah/filesystem/browser.hpp>
#include <djah/filesystem/directory_source.hpp>

#include <djah/log/logger.hpp>
#include <djah/log/console_logger.hpp>


#include "../../collada_converter/sources/collada/proxy.hpp"
#include "../../collada_converter/sources/mesh_builder.hpp"
#include "../../collada_converter/sources/collada/library/common.hpp"
#include "../../collada_converter/sources/collada/utils.hpp"
#include "../../collada_converter/sources/collada/library/visual_scenes.hpp"
#include "../../collada_converter/sources/collada/library/controllers.hpp"

using namespace djah;

//--------------------------------------------------------------------------------------------------
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

	boost::shared_ptr<T> res = resources::resource_manager::get().find<T>(url);
	if(!res && (res = boost::shared_ptr<T>(s_dmm.loadFromUrl<T>(url))))
		resources::resource_manager::get().add(url, res);
	else
		DJAH_BEGIN_LOG(EWL_CRITICAL) << "[ResourceManager] Unable to find the following resource: " << url << DJAH_END_LOG();

	return res;
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
djah::application_base* djah_new_app()
{
	return new viewer_app();
}
//--------------------------------------------------------------------------------------------------

void drawAxis()
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
}

void drawFloor()
{
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

struct bone_t
{
	math::matrix4f bind_shape;
	math::matrix4f bind_pose;
};
typedef std::vector<math::vector3f> vec_list;
typedef std::map<std::string, bone_t> bones_map_t;
bones_map_t bones_map;
static vec_list v;
static math::matrix4f BO[44];
void CreateBones(collada::library::node *n, const math::matrix4f &pm, vec_list &v)
{
	math::matrix4f m(n->matrix_);
	m=pm*m;

	bone_t b = { m, math::matrix4f::mat_identity };
	bones_map[n->sid_] = b;

	if(n->parent_)
	{
		static const math::vector4f w(0.0f,0.0f,0.0f,1.0f);
		v.push_back( math::resize<3>(math::transform(pm, w)) );
		v.push_back( math::resize<3>(math::transform(m, w))  );
	}
	
	collada::library::node_list_t::const_iterator it;
	collada::library::node_list_t::const_iterator it_end = n->children_.end();
	for(it = n->children_.begin(); it != it_end; ++it)
		CreateBones(*it, m, v);
}

//--------------------------------------------------------------------------------------------------
viewer_app::viewer_app()
	: djah::application_base(djah::system::video_config(640,480,32,24,0,false,false))
	, eye_(0,math::pi_over_2,5.8f)
	, center_(0,0,3)
	, up_(0,0,1)
{	
	// File System
	filesystem::browser::get().addSavingChannel (new filesystem::directory_source("."));
	filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("./data"));

	// Logger
	log::logger::setLogger(new djah::log::console_logger);

	time::clock clk;
	collada::proxy obj("data/3d/astroBoy_walk_Max.dae");
	u64 msL = clk.getElapsedTimeMs();

	if( obj.good() )
	{
		clk.restart();

		builder_ = new mesh_builder(obj);

		collada::library::node *root = collada::find_node_by_id(*(obj.getVisualScenes()[0]), "astroBoy_newSkeleton_deformation_rig");
		root = root->children_[0];
		root->parent_ = 0;

		//math::matrix4f id(math::make_rotation(math::deg_to_rad(90.0f), 1.0f, 0.0f, 0.0f));
		math::matrix4f id(math::matrix4f::mat_identity);
		CreateBones(root, id, v);

		collada::library::source *src = collada::get_source_by_id("boyShape-skin-skin-bind_poses", obj.getControllers()[0]->skin_->sources_);

		int bones = 44;
		for(int i = 0; i < bones; ++i)
		{
			std::stringstream ss;
			ss << "Bone" << i+1;
			BO[i] = bones_map[ss.str()].bind_shape;
			float m[16];
			memcpy(m, src->float_array_->data_ + (i*16), sizeof(float)*16);
			BO[i] *= math::matrix4f(m);
		}

		u64 msB = clk.getElapsedTimeMs();

		DJAH_BEGIN_LOG(EWL_NOTIFICATION) << "Loading time : " << msL << " ms" << DJAH_END_LOG();
		DJAH_BEGIN_LOG(EWL_NOTIFICATION) << "Building time : " << msB << " ms" << DJAH_END_LOG();
	}
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
viewer_app::~viewer_app()
{
	delete builder_;
	delete texture_;
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void viewer_app::initImpl()
{
	const float w = static_cast<float>(device_->videoConfig().width);
	const float h = static_cast<float>(device_->videoConfig().height);
	matPerspectiveProj_ = video::make_perspective_projection(60.0f, w/h, 0.1f, 1000.f);
	matOrthoProj_ = video::make_orthographic_projection(0.0f, w, h, 0.0f, -1.0f, 1.0f);
		
	glClearDepth(1.f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	boost::shared_ptr<resources::image> img = find_resource<resources::image>("textures/boy_10.jpg");
	if( img )
	{
		texture_ = new video::ogl::texture(img->width(), img->height());
		texture_->setPixelBuffer(img->pixels(), false);
	}

	video::font_engine::create();
	video::font_engine::get().setFontsPath("fonts/");;
}
//--------------------------------------------------------------------------------------------------

template<typename T>
T interpolate(T a, T b, float t)
{
	return ((1.0f - t) * a) + (t * b);
}
//--------------------------------------------------------------------------------------------------
void viewer_app::runImpl()
{
	static int fps = 0;
	static time::clock clk;

	static float angle = 0.0f;
	eye_.x = 7*cos(angle);
	eye_.y = 7*sin(angle);
	const float da = 0.075f;
	angle = (angle+da > math::pi_times_2) ? 0.0f : angle+da;
	matView_ = video::make_look_at(eye_, center_, up_);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(matPerspectiveProj_.getTransposed().data);
		
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(matView_.getTransposed().data);

	drawAxis();

	/*
	glBegin(GL_LINES);
	glColor3f(1,1,0);
	for(vec_list::iterator it = v.begin(); it != v.end(); ++it)
	{
		glVertex3fv(it->data);
	}
	glEnd();
	*/
	static float t = 0.0f;
	static float s = 1.0f;
	t += 0.1f * s;
	if( t >= 1.0f && s > 0.0f || t <= 0.0f && s < 0.0f )
		s *= -1.0f;
	
	glEnable(GL_TEXTURE_2D);
	if( texture_ )
		texture_->bind();
	glColor3f(1,1,1);
	glBegin(GL_TRIANGLES);
	const int nbSubMeshes = (int)builder_->getSubMeshesCount();
	for(int b = 0; b < nbSubMeshes; ++b)
	{
		const std::vector<float> &buffer = builder_->getBuffer(b);
		const size_t vertexSize = builder_->getVertexSize(b);

		for(size_t i = 0; i < buffer.size(); i += vertexSize)
		{
			math::vector4f pos(buffer[i], buffer[i+1], buffer[i+2], 1.0f);
			math::vector4f norm(buffer[i+3], buffer[i+4], buffer[i+5], 1.0f);
			math::vector4f bones(buffer[i+9], buffer[i+10], buffer[i+11], buffer[i+12]);
			math::vector4f weights(buffer[i+13], buffer[i+14], buffer[i+15], buffer[i+16]);

			math::vector4f p;
			math::vector4f n;
			if( 1 )
			for(int w = 0; w < 4; ++w)
			{
				if( bones.data[w] >= 0.0f && weights.data[w] > 0.0f )
				{
					int bone = static_cast<int>(bones.data[w]);
					const math::matrix4f &mat = BO[bone];

					p += math::transform(mat, pos) * weights.data[w];
					n += math::transform(mat, norm) * weights.data[w];
				}
			}
			else
			{
				p = pos;
				n = norm;
			}
			
			glTexCoord2fv(&buffer[i+6]);
			glVertex3fv(p.data);
			glNormal3fv(n.data);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

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

	
	const float w = static_cast<float>(device_->videoConfig().width);
	const float h = static_cast<float>(device_->videoConfig().height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(matOrthoProj_.getTransposed().data);
		
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	fps_str_.draw();
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void viewer_app::exitImpl()
{
}
//--------------------------------------------------------------------------------------------------