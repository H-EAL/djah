#include "solar_system_test.hpp"
#include "djah/types.hpp"
#include "resource_finder.hpp"
#include "djah/3d/primitives.hpp"
#include "djah/resources/media_manager.hpp"

using namespace djah;
using namespace d3d;

//--------------------------------------------------------------------------------------------------
SolarSystemTest::SolarSystemTest(djah::system::device_ptr pDevice, const djah::system::input::gamepad &g, Camera &cam)
	: test_base(pDevice)
	, cam_(cam)
	, gamepad_(g)
	, pSphereVB_(nullptr)
	, pSphereVA_(nullptr)
	, shader_("lit_textured")
{
	const float w = static_cast<float>(pDevice_->videoConfig().width);
	const float h = static_cast<float>(pDevice_->videoConfig().height);
	matPerspectiveProj_ = math::make_perspective_projection(60.0f, w/h, 0.1f, 1000.f);

	primitives::iso_sphere p;
	const std::vector<primitives::triangle> &triangles = p.construct(3);
	pSphereVB_ = new opengl::vertex_buffer(triangles.size() * sizeof(primitives::triangle), opengl::eBU_StaticDraw);
	pSphereVB_->write(&triangles[0], triangles.size());

	opengl::vertex_format vertexFormat;
	vertexFormat.record()
		<< opengl::format::position<3,float>()
		<< opengl::format::normal<3,float>()
		<< opengl::format::tex_coord<2,float>();

	pSphereVA_ = new opengl::vertex_array(vertexFormat, pSphereVB_);
	pSphereVA_->init(shader_.program());

	const std::string textureFiles[] = { "sun.png", "earth.jpg", "moon.jpg" };
	for(int i = 0; i < ePO_Count; ++i)
	{
		resources::image_ptr img = find_resource<resources::image>("textures/" + textureFiles[i]);
		if( img )
		{
			GLenum internalFormat = img->channels() == 1 ? GL_DEPTH_COMPONENT : GL_RGB;
			pTextures_[i] = new opengl::texture(internalFormat, img->width(), img->height(), true);
			pTextures_[i]->bind();
			pTextures_[i]->setWrapMode(GL_CLAMP_TO_EDGE);
			pTextures_[i]->setBestFiltering();
			GLenum pixelFormat = img->channels() == 1 ? GL_DEPTH_COMPONENT : GL_BGR;			
			pTextures_[i]->setPixelBuffer(pixelFormat, GL_UNSIGNED_BYTE, img->pixels());
		}
	}
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
SolarSystemTest::~SolarSystemTest()
{
	delete pSphereVB_;
	delete pSphereVA_;
	for(int i = 0; i < ePO_Count; ++i)
		delete pTextures_[i];
}
//--------------------------------------------------------------------------------------------------

static float sun_rotation   = 0.0f;
static float earth_rotation = 0.0f;
static float moon_rotation  = 0.0f;
static float earth_orbite   = 0.0f;
static float moon_orbite    = 0.0f;
template<typename T>
T rotate(T toRotate, T _min, T _max)
{
	return toRotate > _max ? _min : toRotate;
}
//--------------------------------------------------------------------------------------------------
void SolarSystemTest::update(float dt)
{
	static bool animate = true;

	if( gamepad_.getButton(system::input::eX360_Start).pressed() )
		animate = !animate;

	if( !animate )
		return;

	sun_rotation   += math::deg_to_rad( 0.0f) * dt;
	earth_rotation += math::deg_to_rad(15.0f) * dt;
	moon_rotation  += math::deg_to_rad(40.0f) * dt;
	earth_orbite   += math::deg_to_rad(15.0f) * dt;
	moon_orbite    += math::deg_to_rad(50.0f) * dt;

	sun_rotation   = rotate(sun_rotation  , 0.0f, math::pi_times_2);
	earth_rotation = rotate(earth_rotation, 0.0f, math::pi_times_2);
	moon_rotation  = rotate(moon_rotation , 0.0f, math::pi_times_2);
	earth_orbite   = rotate(earth_orbite  , 0.0f, math::pi_times_2);
	moon_orbite    = rotate(moon_orbite   , 0.0f, math::pi_times_2);
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void SolarSystemTest::draw()
{
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);

	opengl::frame_buffer::unbind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const math::matrix4f &matVP = math::make_look_at(cam_.eye(), cam_.center(), cam_.up()) * matPerspectiveProj_;
	
	math::matrix4f matWorld[ePO_Count];
	matWorld[ePO_Sun]
		= math::make_scale(3.0f)
		* math::make_rotation(sun_rotation, math::vector3f::y_axis);

	matWorld[ePO_Earth]
		= math::make_scale(1.0f)
		* math::make_rotation(earth_rotation, math::vector3f::y_axis)
		* math::make_translation(10.0f, 0.0f, 0.0f)
		* math::make_rotation(earth_orbite, math::vector3f::y_axis);

	matWorld[ePO_Moon]
		= math::make_scale(0.5f)
		* math::make_rotation(moon_rotation, math::vector3f::y_axis)
		* math::make_translation(4.0f, 0.0f, 0.0f)
		* math::make_rotation(moon_orbite, math::vector3f::y_axis)
		* matWorld[ePO_Earth];
	
	shader_.program().begin();
	shader_.program().sendUniform("in_UsePointLight", 1);
	shader_.program().sendUniform("in_DiffuseSampler", 0);
	for(int i = 0; i < ePO_Count; ++i)
	{
		shader_.program().sendUniformMatrix( "in_World", matWorld[i] );
		shader_.program().sendUniformMatrix( "in_WVP", matWorld[i] * matVP);
		pTextures_[i]->bind();
		pSphereVA_->draw();
	}
	opengl::texture::unbind();
	shader_.program().end();

	glDisable(GL_DEPTH_TEST);
}
//--------------------------------------------------------------------------------------------------