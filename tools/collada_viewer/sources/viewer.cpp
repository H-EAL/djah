#include "viewer.hpp"

#include <djah/application_base.hpp>
#include <djah/core/time/timer.hpp>
#include <djah/core/randomizer.hpp>
#include <djah/math.hpp>

#include <djah/system/video_config.hpp>
#include <djah/system/gl.hpp>
#include <djah/system/device.hpp>

#include <djah/opengl.hpp>
#include <djah/3d/font_engine.hpp>

#include <djah/resources/resource_manager.hpp>
#include <djah/resources/media_manager.hpp>
#include <djah/resources/image_loader.hpp>

#include <djah/filesystem/browser.hpp>
#include <djah/filesystem/directory_source.hpp>

#include <djah/debug/log.hpp>
#include <djah/debug/console_sink.hpp>


#include "collada/proxy.hpp"
#include "collada/library/common.hpp"
#include "collada/utils.hpp"
#include "collada/library/visual_scenes.hpp"
#include "collada/library/controllers.hpp"
#include "../../collada_converter/sources/mesh_builder.hpp"

using namespace djah;

const bool bAstro = true;
std::string dae_file = "data/3d/dude.dae";
std::string tex_file = "textures/bb.jpg";
const float scale = 0.5f;
const bool bRotate = false;
const bool bScale = true;
const float rotSpeed = 1.0f*360.0f / 10.0; //(deg/s)
const float camRadius = 7.0f;
const bool bSkin = true;
const bool drawModel = true;

//--------------------------------------------------------------------------------------------------
template<typename T>
std::shared_ptr<T> find_resource(const std::string &url)
{
	static resources::default_media_manager s_dmm;
	static bool initialized = false;
	if(!initialized)
	{
		s_dmm.registerLoader( new resources::image_loader, "png tga jpg");
		initialized = true;
	}

	std::shared_ptr<T> res = resources::resource_manager::get().find<T>(url);
	if(!res && (res = std::shared_ptr<T>(s_dmm.loadFromUrl<T>(url))))
		resources::resource_manager::get().add(url, res);
	else
		DJAH_BEGIN_LOG(EWL_CRITICAL) << "[ResourceManager] Unable to find the following resource: " << url << DJAH_END_LOG();

	return res;
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
	glPushMatrix();
	static const float s = 50.0f;
	static const float c = 0.8f;
	static const float h = -0.1f;
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
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
	glPopMatrix();
}

void drawGrid(int w = 4, int h = 4, float cw = 1.0f, float ch = 1.0f)
{
	glPushMatrix();
	glBegin(GL_LINES);
	static const float e = -0.0001f;
	static const float c = 0.3f;
	glColor3f(c,c,c);
	for(int x = -w/2; x < w/2+1; ++x)
	{
		glVertex3f((float)x*cw, (float)h/2*ch, e);
		glVertex3f((float)x*cw, (float)-h/2*ch, e);
	}
	for(int y = -h/2; y < h/2+1; ++y)
	{
		glVertex3f((float)w/2*cw, (float)y*ch, e);
		glVertex3f((float)-w/2*cw, (float)y*ch, e);
	}
	glEnd();
	glPopMatrix();
}

math::matrix4f tomat(const collada::library::transformation_list_t (&trans)[3])
{
	math::matrix4f mat;
	mat.toIdentity();

	for(unsigned int r = 0; r < trans[collada::library::transformation::ETT_ROTATE].size(); ++r)
	{
		const collada::library::transformation &rot = *trans[collada::library::transformation::ETT_ROTATE][r];
		mat *= math::make_rotation(math::deg_to_rad(rot.values_[2]), rot.values_[0], rot.values_[1], rot.values_[2]);
	}

	for(unsigned int t = 0; t < trans[collada::library::transformation::ETT_TRANSLATE].size(); ++t)
	{
		const collada::library::transformation &tr = *trans[collada::library::transformation::ETT_TRANSLATE][t];
		mat *= math::make_translation(tr.values_[0], tr.values_[1], tr.values_[2]);
	}

	for(unsigned int s = 0; s < trans[collada::library::transformation::ETT_SCALE].size(); ++s)
	{
		const collada::library::transformation &sc = *trans[collada::library::transformation::ETT_SCALE][s];
		mat *= math::make_scale(sc.values_[0], sc.values_[1], sc.values_[2]);
	}

	return mat;
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
void CreateBones(collada::library::node *n, const math::matrix4f &pm/*, vec_list &v*/)
{
	math::matrix4f m(n->matrix_);
	//math::matrix4f m = tomat(n->transformations_);
	m=pm*m;

	bone_t b = { m, math::matrix4f::identity };
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
		CreateBones(*it, m/*, v*/);
}
collada::proxy *obj = 0;
//--------------------------------------------------------------------------------------------------
viewer_app::viewer_app()
	: djah::application_base(djah::system::video_config(1440,810,32,24,0,false,true))
	, texture_(0)
	, eye_(0,0,1.85f)
	, center_(0,0,1.5f)
	, up_(0,0,1)
{	
	// File System
	filesystem::browser::get().addSavingChannel (new filesystem::directory_source("."));
	filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("./data"));

	// Logger
	debug::logger::setLogger(new djah::debug::console_logger);

	time::timer clk;
	obj = new collada::proxy(bAstro ? "data/3d/astroBoy_walk_Max.dae" : dae_file);
	f32 msL = clk.getElapsedTimeMs();

	if( obj->good() )
	{
		clk.restart();

		builder_ = new mesh_builder(*obj);
		/**
		collada::library::node *root = collada::find_node_by_sid(*(obj.getVisualScenes()[0]), "Bone1");
		//collada::library::node *root = collada::find_node_by_id(*(obj.getVisualScenes()[0]), "Root");
		//root = root->children_[0];
		root->parent_ = 0;

		//math::matrix4f id(math::make_rotation(math::deg_to_rad(90.0f), 1.0f, 0.0f, 0.0f));
		math::matrix4f id(math::matrix4f::mat_identity);
		CreateBones(root, id, v);
		
		skeleton *skel = builder_->getMesh(0)->skeleton_;
		if( skel )
		{
			int bones = skel->bones_.size();
			for(int i = 0; i < bones; ++i)
				BO[i] = bones_map[ skel->bones_[i].name_ ].bind_shape * skel->bones_[i].inv_bind_matrix_;
		}
		/**/
		f32 msB = clk.getElapsedTimeMs();

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
	matPerspectiveProj_ = math::make_perspective_projection(60.0f, w/h, 0.1f, 10000.f);
	matOrthoProj_ = math::make_orthographic_projection(0.0f, w, h, 0.0f, -1.0f, 1.0f);
		
	glClearDepth(1.f);
	glClearColor(0.0f,1.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	resources::image_ptr img = find_resource<resources::image>(bAstro ? "textures/boy_10.jpg" : tex_file);
	if( img )
	{
		texture_ = new video::ogl::texture(GL_RGB, img->width(), img->height());
		texture_->setPixelBuffer(img->pixels(), GL_BGR, GL_UNSIGNED_BYTE);
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
	static time::timer clk;
	static time::timer clkDT;

	float dt = clkDT.getElapsedTimeSec();
	clkDT.restart();
	static float angle = 0.0f;
	eye_.x = camRadius*cos(angle+math::pi_over_2);
	eye_.y = camRadius*sin(angle+math::pi_over_2);
	const float da = math::deg_to_rad( dt * rotSpeed );
	angle = (angle+da > math::pi_times_2) ? angle+da-math::pi_times_2 : angle+da;
	matView_ = math::make_look_at(eye_, center_, up_);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(matPerspectiveProj_[0]);
		
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(matView_[0]);

	drawAxis();
	drawGrid();

	float times[] = {0.0f,0.033333f,0.066666f,0.1f,0.133333f,0.166667f,0.2f,0.233333f,0.266667f,0.3f,0.333333f,0.366667f,0.4f,0.433333f,0.466667f,0.5f,0.533333f,0.566667f,0.6f,0.633333f,0.666667f,0.7f,0.733333f,0.766667f,0.8f,0.833333f,0.866667f,0.9f,0.933333f,0.966667f,1.0f,1.03333f,1.06667f,1.1f,1.13333f,1.16667f};
	static int k = -1;
	static const int nbk = sizeof(times)/sizeof(float);
	if(++k >= nbk)
		k = 0;

	static float t = 0.0f;
	static float s = 1.0f;
	t += 0.1f * s;
	if( t >= 1.0f && s > 0.0f || t <= 0.0f && s < 0.0f )
		s *= -1.0f;

	glPushMatrix();
	glLineWidth(3.0f);
	if(bRotate)
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	if(bScale)
		glScalef(scale,scale,scale);
	static bool done = false;
	const int nbModels = (int)builder_->getModelCount();
	for(int m = 0; m < nbModels; ++m)
	{
		model *mod = builder_->getModel(m);
		const int nbMeshes = (int)mod->getMeshCount();
		for(int b = 0; b < nbMeshes; ++b)
		{
			mesh_t *msh = mod->getMesh(b);
			if( msh->skeleton_ != 0 )
			{
				if(!done){
				msh->skeleton_->setupBonesHierarchy(*(obj->getVisualScenesLib()), true, times[k]);
				done = true;
				}
				msh->setupBindPose();

				glBegin(GL_LINES);
				skeleton::bone_list_t::const_iterator it;
				int i = -1;
				for(it = msh->skeleton_->bones_.begin(); it != msh->skeleton_->bones_.end(); ++it)
				{
					if( ++i == 0 )
						continue;
					glColor3f(0,1,0);
					glVertex3fv(it->start_.data);
					glColor3f(1,0,0);
					glVertex3fv(it->end_.data);
				}
				glEnd();
			}
		}
	}
	done = false;
	glLineWidth(1.0f);
	glPopMatrix();
	/**/
	if( drawModel )
	{
	if( texture_ )
	{
		glEnable(GL_TEXTURE_2D);
		texture_->bind();
	}
	glColor3f(1,1,1);
	glPushMatrix();
	if(bRotate)
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	if(bScale)
		glScalef(scale,scale,scale);
	glBegin(GL_TRIANGLES);

	for(int m = 0; m < nbModels; ++m)
	{
		model *mod = builder_->getModel(m);
		const int nbMeshes = (int)mod->getMeshCount();
		for(int b = 0; b < nbMeshes; ++b)
		{
			mesh_t *msh = mod->getMesh(b);
			bool skined = msh->skeleton_ != 0;//rand() % 101 > 20;

			int pos_stride		= msh->getPositionStride();
			int norm_stride		= msh->getNormalStride();
			int tex_stride		= msh->getTexCoordStride();
			int weight_stride	= msh->getWeightStride();
			int infl_stride		= msh->getInfluenceStride();

			const std::vector<float> &positions				= msh->positions_;
			const std::vector<float> &normals				= msh->normals_;
			const std::vector<float> &tex_coords			= msh->tex_coords_;
			const std::vector<float> &skinned_positions		= msh->skinned_positions_;
			const std::vector<float> &skinned_normals		= msh->skinned_normals_;
			const std::vector<float> &weights				= msh->weights_;
			const std::vector<unsigned short> &influences	= msh->influences_;

			const int nbVertex = msh->vertex_count_;
			for(int i = 0; i < nbVertex; ++i)
			{
				if( bSkin && skined )
				{
					if(!tex_coords.empty())
						glTexCoord2fv(&tex_coords[i*tex_stride]);
					if(!skinned_normals.empty())
						glNormal3fv(&skinned_normals[i*norm_stride]);
					glVertex3fv(&skinned_positions[i*pos_stride]);
				}
				else
				{
					if(!tex_coords.empty())
						glTexCoord2fv(&tex_coords[i*tex_stride]);
					if(!normals.empty())
						glNormal3fv(&normals[i*norm_stride]);
					glVertex3fv(&positions[i*pos_stride]);
				}
			}
		}
	}
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	}
	/**/
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

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(matOrthoProj_[0]);
		
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