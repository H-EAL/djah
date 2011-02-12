#include <iostream>

#include <djah/video/projection.hpp>
#include <djah/video/device_base.hpp>
#include <djah/video/drivers/ogl.hpp>
#include <djah/math.hpp>
#include <djah/types.hpp>

#include <djah/log/logger.hpp>
#include <djah/log/console_logger.hpp>
#include <djah/log/html_logger.hpp>

#include <djah/fs/filesystem.hpp>
#include <djah/fs/directory_source.hpp>
#include <djah/fs/memory_stream.hpp>
#include <djah/fs/compression_techniques.hpp>

#include <djah/resources/media_manager.hpp>
#include <djah/resources/resource_manager.hpp>
#include <djah/resources/loaders/image_loader.hpp>

#include <djah/utils/randomizer.hpp>
#include <djah/time/clock.hpp>

#include "collada/proxy.hpp"
#include "collada/library/geometries.hpp"
#include "collada/library/visual_scenes.hpp"
#include "collada/library/images.hpp"
#include "collada/library/common.hpp"
#include "collada/utils.hpp"

#include "mesh_builder.hpp"

using namespace djah;
using namespace djah::video::drivers;


void printInfosAux()
{
	std::string extensions("");
	ogl::capabilities::string_list_t::const_iterator it    = ogl::capabilities::s_extensions_.begin();
	ogl::capabilities::string_list_t::const_iterator itEnd = ogl::capabilities::s_extensions_.end();
	for(;it != itEnd; ++it)
		extensions += "| " + (*it) + "\n";

	log::logger::log(log::EWL_NOTIFICATION)
		<< "===========================================================================\n"
		<< "| Renderer                 | " << ogl::capabilities::renderer()        << "\n"
		<< "| Vendor                   | " << ogl::capabilities::vendor()          << "\n"
		<< "| OpenGL version           | " << ogl::capabilities::opengl_version()  << "\n"
		<< "| GLSL version             | " << ogl::capabilities::glsl_version()    << "\n"
		<< "---------------------------------------------------------------------------\n"
		<< "| Available extensions (" << ogl::capabilities::s_extensions_.size()  << ")\n"
		<< "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n"
		<< extensions
		<< "==========================================================================="
		<< log::logger::endl();
}


void drawAxis()
{
	glBegin(GL_LINES);
	{
		//X
		glColor3fv (math::vector3f::x_axis.data	);
		glVertex3fv(math::vector3f::null_vector.data	);
		glVertex3fv(math::vector3f::x_axis.data	);
		//Y
		glColor3fv (math::vector3f::y_axis.data	);
		glVertex3fv(math::vector3f::null_vector.data	);
		glVertex3fv(math::vector3f::y_axis.data	);
		//Z
		glColor3fv (math::vector3f::z_axis.data	);
		glVertex3fv(math::vector3f::null_vector.data	);
		glVertex3fv(math::vector3f::z_axis.data	);
	}
	glEnd();
}


template<typename T>
boost::shared_ptr<T> find_resource(const std::string &url)
{
	static resources::default_media_manager s_dmm;
	static bool initialized = false;
	if(!initialized)
	{
		s_dmm.registerLoader( new resources::loaders::image_loader, "png tga jpg");
		initialized = true;
	}

	boost::shared_ptr<T> res = resources::resource_manager::get_instance().get<T>(url);
	if(!res && (res = boost::shared_ptr<T>(s_dmm.loadFromUrl<T>(url))))
		resources::resource_manager::get_instance().add(url, res);
	else
		DJAH_BEGIN_LOG(EWL_CRITICAL) << "[ResourceManager] Unable to find the following resource: " << url << DJAH_END_LOG();

	return res;
}


math::matrix4f get_matrix(collada::library::node *n)
{
	return n->matrix_;

	float *tr = n->transformations_[collada::library::transformation::ETT_TRANSLATE][0]->values_;
	math::matrix4f m = math::make_translation(tr[0], tr[1], tr[2]);

	collada::library::transformation_list_t::const_iterator it	 = n->transformations_[collada::library::transformation::ETT_ROTATE].begin();
	collada::library::transformation_list_t::const_iterator it_end = n->transformations_[collada::library::transformation::ETT_ROTATE].end();
	for(; it != it_end; ++it)
	{
		float angle = (*it)->values_[3];

		math::vector3f axis = math::vector3f::null_vector;

		if((*it)->sid_ == "rotateAxisX")
			axis = math::vector3f::x_axis;
		else if((*it)->sid_ == "rotateY")
			axis = math::vector3f::y_axis;
		else if((*it)->sid_ == "rotateZ")
			axis = math::vector3f::z_axis;

		if(axis != math::vector3f::null_vector)
			m *= math::make_rotation(math::deg_to_rad(angle), axis);
	}

	return m;
}

typedef std::vector<math::vector3f> vec_list;


void renderNode(collada::library::node *n, const math::matrix4f &pm, vec_list &v, const std::string &sp = "")
{
	math::matrix4f m = get_matrix(n);
	m.transpose();
	m=pm*m;

	if(n->parent_)
	{
		math::vector4f w(0.0f,0.0f,0.0f,1.0f);
		math::vector4f v4 = math::transform(m, w);
		math::vector3f pos = math::vec4_to_vec3(v4);
		v4 = math::transform(pm, w);
		math::vector3f ppos = math::vec4_to_vec3(v4);
		v.push_back(ppos);
		v.push_back(pos);
	}
	
	collada::library::node_list_t::const_iterator it;
	collada::library::node_list_t::const_iterator it_end = n->children_.end();
	for(it = n->children_.begin(); it != it_end; ++it)
		renderNode(*it, m, v, sp+"  ");
}


int main(int argc, char *argv[])
{

	// File System
	djah::fs::filesystem::get_instance().addSavingChannel (new djah::fs::directory_source("."));
	djah::fs::filesystem::get_instance().addLoadingChannel(new djah::fs::directory_source("../data"));
	djah::fs::filesystem::get_instance().addLoadingChannel(new djah::fs::directory_source("../data/3d"));
	djah::fs::filesystem::get_instance().addLoadingChannel(new djah::fs::directory_source("../data/textures"));
	djah::fs::filesystem::get_instance().addLoadingChannel(new djah::fs::pak_source("2pak.pak"));

	// Logger
	djah::log::logger::setLogger(new djah::log::console_logger);
	/*for(int i = 0; i < 32; ++i)
	{
		int level = djah::utils::randomizer::random(4);
		log::logger::log(log::logger::E_WARNING_LEVEL(level)) << "level " << level << log::logger::endl();
	}*/

	time::clock clk;
	clk.restart();
	collada::proxy obj(/**/"../data/3d/astroBoy_walk_Max.dae"/**/ /**"../../data/3d/seymour_triangulate.dae"/**/);
	u64 msL = clk.getElapsedTimeMs();

	if( !obj.good() )
		return 1;

	clk.restart();

	mesh_builder mesh(obj);

	collada::library::node *root = collada::find_node_by_id(*(obj.getVisualScenes()[0]), /**"group1"/**/ /**/"astroBoy_newSkeleton_deformation_rig"/**/);
	root = root->children_[0];
	root->parent_ = 0;

	math::matrix4f id;
	id = math::make_translation(2.5f, 0.0f, 0.0f);
	vec_list v;
	renderNode(root, id, v);

	u64 msB = clk.getElapsedTimeMs();

	DJAH_BEGIN_LOG(EWL_NOTIFICATION) << "Loading time : " << msL << " ms" << DJAH_END_LOG();
	DJAH_BEGIN_LOG(EWL_NOTIFICATION) << "Building time : " << msB << " ms" << DJAH_END_LOG();

	/*fs::memory_stream mstrm(fs::filesystem::get_instance().openReadStream("test.frag"));
	std::cout << mstrm.toString();*/


	// Rendering device
	const int s = 1;
	djah::video::device_ptr device = djah::video::create_device(480*s, 320*s/*, 24, 24, 0, false, true*/);
	djah::video::driver_ptr driver = device->videoDriver();
	//printInfosAux();

	float aspect = static_cast<float>(device->videoConfig().width_) / static_cast<float>(device->videoConfig().height_);
	math::matrix4f proj = video::make_perspective_projection(60.0f, aspect, 0.1f, 1000.0f);
	driver->setProjectionMatrix(proj);

	/**/
	glEnable(GL_LIGHTING);	// Active l'éclairage
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	int LightPos[4] = {0,10,5,1};
	glLightiv(GL_LIGHT0,GL_POSITION,LightPos);

	boost::shared_ptr<resources::image> img = find_resource<resources::image>(obj.getImages()[0]->init_from_filename_);
	GLuint tex = 0;
	if(img)
	{
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1,&tex);	//Génère un n° de texture
		glBindTexture(GL_TEXTURE_2D,tex);	//Sélectionne ce n°
		glTexImage2D (
			GL_TEXTURE_2D,	//Type : texture 2D
			0,	//Mipmap : aucun
			GL_RGB,	//Couleurs : 4
			img->width(),	//Largeur : 2
			img->height(),	//Hauteur : 2
			0,	//Largeur du bord : 0
			GL_BGR,	//Format : RGBA
			GL_UNSIGNED_BYTE,	//Type des couleurs
			img->pixels()	//Addresse de l'image
		);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	/**/

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	math::matrix4f view;
	view.identity();
	view *= math::make_translation(0.0f, -3.5f, -10.0f);
	view *= math::make_rotation(math::deg_to_rad(-90.0f), math::vector3f::x_axis);
	view *= math::make_rotation(math::deg_to_rad(-90.0f), math::vector3f::z_axis);

	while( device->run() )
	{
		view *= math::make_rotation(math::deg_to_rad(clk.getElapsedTimeSec()*100.0f/2.5f), math::vector3f::z_axis);
		driver->setViewMatrix(view);

		clk.restart();
		driver->beginScene();
		{
			//drawAxis();

			glBegin(GL_LINES);
			glColor3f(1,0,0);
			for(vec_list::iterator it = v.begin(); it != v.end(); ++it)
			{
				glVertex3fv(it->data);
			}
			glEnd();

			/**/
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,tex);
			glEnable(GL_LIGHTING);
			glTranslatef(-2.5f,0,0);
			const int nbSubMeshes = (int)mesh.getSubMeshesCount();
			for(int b = 0; b < nbSubMeshes; ++b)
			{
				const std::vector<float> &buffer = mesh.getBuffer(b);
				const size_t vertexSize = mesh.getVertexSize(b);

				glBegin(GL_TRIANGLES);
				for(size_t i = 0; i < buffer.size(); i += vertexSize)
				{
					glTexCoord2fv(&buffer[i+6]);
					glNormal3fv(&buffer[i+3]);
					glVertex3fv(&buffer[i]);
				}
				glEnd();
			}
			/**/
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
		}
		driver->endScene();
		std::stringstream str;
		str << clk.getElapsedTimeMs() << " mssssssss";
		device->setWindowTitle(str.str());

		device->swapBuffers();
	}

	device->shutDown();

	djah::log::logger::setLogger(0);

	return 0;
}