#include <fstream>
#include "collada/proxy.hpp"
#include "mesh_builder.hpp"

int main()
{
	collada::proxy obj("data/3d/feisar.dae");
	if( !obj.good() )
		return EXIT_FAILURE;

	std::fstream file;
	file.open("data/3d/feisar.bdae", std::ios::in | std::ios::binary | std::ios::trunc | std::ios::out);
	if( !file.good() )
		return EXIT_FAILURE;

	mesh_builder builder(obj);

	const int nbModels = (int)builder.getModelCount();
	for(int m = 0; m < nbModels; ++m)
	{
		model *mod = builder.getModel(m);
		const int nbMeshes = (int)mod->getMeshCount();
		for(int b = 0; b < nbMeshes; ++b)
		{
			mesh_t *msh = mod->getMesh(b);
			const std::vector<float>			&positions		= msh->positions_;
			const std::vector<float>			&normals		= msh->normals_;
			const std::vector<float>			&tex_coords		= msh->tex_coords0_;
			const std::vector<float>			&tex_tangents	= msh->tex_tangents_;
			const std::vector<float>			&tex_binormals	= msh->tex_binormals_;
			const std::vector<float>			&weights		= msh->weights_;
			const std::vector<unsigned short>	&influences		= msh->influences_;
			const size_t vertexSize								= msh->vertex_size_;

			const int pos_stride			= msh->getPositionStride();
			const int norm_stride			= msh->getNormalStride();
			const int tex_coord_stride		= msh->getTexCoordStride();
			const int tex_tangent_stride	= msh->getTexTangentStride();
			const int tex_binormal_stride	= msh->getTexBinormalStride();
			const int weight_stride			= msh->getWeightStride();
			const int infl_stride			= msh->getInfluenceStride();

			if( !positions.empty() )
			{
				for(int i = 0; i < msh->vertex_count_; ++i)
				{
					file.write((const char*)&positions[i*pos_stride], pos_stride * sizeof(float));

					if( !normals.empty() )
						file.write((const char*)&normals[i*norm_stride], norm_stride * sizeof(float));

					if( !tex_coords.empty() )
						file.write((const char*)&tex_coords[i*tex_coord_stride], tex_coord_stride * sizeof(float));
					/**/
					if( !tex_tangents.empty() )
						file.write((const char*)&tex_tangents[i*tex_tangent_stride], tex_tangent_stride * sizeof(float));

					if( !tex_binormals.empty() )
						file.write((const char*)&tex_binormals[i*tex_binormal_stride], tex_binormal_stride * sizeof(float));

					if( !weights.empty() )
						file.write((const char*)&weights[i*weight_stride], weight_stride * sizeof(float));

					if( !influences.empty() )
						file.write((const char*)&influences[i*infl_stride], infl_stride * sizeof(unsigned short));
					/**/
				}
			}
		}
	}

	return EXIT_SUCCESS;
}

/*
#include <djah/video/projection.hpp>
#include <djah/video/device_base.hpp>
#include <djah/video/drivers/ogl.hpp>
#include <djah/math.hpp>
#include <djah/types.hpp>

#include <djah/log/logger.hpp>
#include <djah/log/console_logger.hpp>
#include <djah/log/html_logger.hpp>

#include <djah/filesystem/browser.hpp>
#include <djah/filesystem/directory_source.hpp>
#include <djah/filesystem/memory_stream.hpp>
#include <djah/filesystem/compression_techniques.hpp>

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
using namespace djah::math;
using namespace djah::video;
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
		glColor3fv (math::vector3f::x_axis.data		);
		glVertex3fv(math::vector3f::null_vector.data);
		glVertex3fv(math::vector3f::x_axis.data		);
		//Y
		glColor3fv (math::vector3f::y_axis.data		);
		glVertex3fv(math::vector3f::null_vector.data);
		glVertex3fv(math::vector3f::y_axis.data		);
		//Z
		glColor3fv (math::vector3f::z_axis.data		);
		glVertex3fv(math::vector3f::null_vector.data);
		glVertex3fv(math::vector3f::z_axis.data		);
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

	boost::shared_ptr<T> res = resources::resource_manager::get().find<T>(url);
	if(!res && (res = boost::shared_ptr<T>(s_dmm.loadFromUrl<T>(url))))
		resources::resource_manager::get().add(url, res);
	else
		DJAH_BEGIN_LOG(EWL_CRITICAL) << "[ResourceManager] Unable to find the following resource: " << url << DJAH_END_LOG();

	return res;
}

typedef std::vector<math::vector3f> vec_list;


void CreateBones(collada::library::node *n, const matrix4f &pm, vec_list &v)
{
	matrix4f m(n->matrix_);
	m.transpose();
	m=pm*m;

	if(n->parent_)
	{
		static const vector4f w(0.0f,0.0f,0.0f,1.0f);
		v.push_back( math::resize<3>(math::transform(pm, w)) );
		v.push_back( math::resize<3>(math::transform(m, w))  );
	}
	
	collada::library::node_list_t::const_iterator it;
	collada::library::node_list_t::const_iterator it_end = n->children_.end();
	for(it = n->children_.begin(); it != it_end; ++it)
		CreateBones(*it, m, v);
}


int main(int argc, char *argv[])
{
	// File System
	djah::filesystem::browser::get().addSavingChannel (new djah::filesystem::directory_source("."));
	djah::filesystem::browser::get().addLoadingChannel(new djah::filesystem::directory_source("../data"));
	djah::filesystem::browser::get().addLoadingChannel(new djah::filesystem::directory_source("../data/3d"));
	djah::filesystem::browser::get().addLoadingChannel(new djah::filesystem::directory_source("../data/textures"));
	djah::filesystem::browser::get().addLoadingChannel(new djah::filesystem::pak_source("2pak.pak"));

	// Logger
	log::logger::setLogger(new djah::log::console_logger);

	time::clock clk;
	collada::proxy obj(/**"../data/3d/astroBoy_walk_Max.dae"/**/ /**"../../data/3d/seymour_triangulate.dae"/**);
	u64 msL = clk.getElapsedTimeMs();

	if( !obj.good() )
		return 1;

	clk.restart();

	mesh_builder mesh(obj);

	collada::library::node *root = collada::find_node_by_id(*(obj.getVisualScenes()[0]), /**"group1"/** /**"astroBoy_newSkeleton_deformation_rig"/**);
	root = root->children_[0];
	root->parent_ = 0;

	matrix4f id = math::make_translation(2.5f, 0.0f, 0.0f);
	vec_list v;
	CreateBones(root, id, v);

	u64 msB = clk.getElapsedTimeMs();

	DJAH_BEGIN_LOG(EWL_NOTIFICATION) << "Loading time : " << msL << " ms" << DJAH_END_LOG();
	DJAH_BEGIN_LOG(EWL_NOTIFICATION) << "Building time : " << msB << " ms" << DJAH_END_LOG();


	// Rendering device
	const int s = 1;
	device_ptr device = create_device(480*s, 320*s/*, 24, 24, 0, false, true*);
	driver_ptr driver = device->videoDriver();
	//printInfosAux();

	float aspect = static_cast<float>(device->videoConfig().width) / static_cast<float>(device->videoConfig().height);
	matrix4f proj = video::make_perspective_projection(60.0f, aspect, 0.1f, 1000.0f);
	driver->setProjectionMatrix(proj);

	/**
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
	/**

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	matrix4f view(matrix4f::mat_identity);
	//view *= math::make_translation(0.0f, -3.5f, -10.0f);
	//view *= math::make_rotation(math::deg_to_rad(90.0f), math::vector3f::x_axis);
	//view *= math::make_rotation(math::deg_to_rad(-90.0f), math::vector3f::z_axis);

	while( device->run() )
	{
		//view *= math::make_rotation(math::deg_to_rad(clk.getElapsedTimeSec()*100.0f/2.5f), math::vector3f::z_axis);
		driver->setViewMatrix(view);

		clk.restart();
		driver->beginScene();
		{
			drawAxis();

			glBegin(GL_LINES);
			glColor3f(1,0,0);
			for(vec_list::iterator it = v.begin(); it != v.end(); ++it)
			{
				glVertex3fv(it->data);
			}
			glEnd();

			/**
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
			/**
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
		}
		driver->endScene();

		device->swapBuffers();
	}

	device->shutDown();

	djah::log::logger::setLogger(0);

	return 0;
}
*/