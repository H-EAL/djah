#include "solar_system_test.hpp"
#include "djah/types.hpp"
#include "resource_finder.hpp"
#include "djah/3d/primitives.hpp"
#include "djah/3d/lights.hpp"
#include "djah/resources/asset_finder.hpp"

using namespace djah;
using namespace d3d;

//--------------------------------------------------------------------------------------------------
SolarSystemTest::SolarSystemTest(djah::system::device_sptr pDevice, const djah::system::input::gamepad &g, Camera &cam)
	: test_base(pDevice)
	, cam_(cam)
	, gamepad_(g)
	, pSphereVB_(nullptr)
	, shader_("lit_textured")
{
	const float w = static_cast<float>(pDevice_->config().width);
	const float h = static_cast<float>(pDevice_->config().height);
	matPerspectiveProj_ = math::make_perspective_projection(60.0f, w/h, 0.1f, 1000.f);

	primitives::sphere p;
	const std::vector<primitives::triangle> &triangles = p.construct(10,10);
	pSphereVB_ = new opengl::vertex_buffer(triangles.size() * sizeof(primitives::triangle), opengl::eBU_StaticDraw);
	pSphereVB_->write(&triangles[0], triangles.size());

	opengl::vertex_format vertexFormat;
	vertexFormat.record()
		<< opengl::format::position<3,float>()
		<< opengl::format::normal<3,float>()
		<< opengl::format::tex_coord<2,float>();

	sphereVA_.addVertexBuffer(pSphereVB_, vertexFormat);
	sphereVA_.setVertexCount(triangles.size()*3);
	sphereVA_.init(shader_.program());

	const std::string textureFiles[] = { "sun.png", "earth.jpg", "moon.jpg" };
	for(int i = 0; i < ePO_Count; ++i)
	{
		pTextures_[i] = d3d::texture_manager::get().find(textureFiles[i]);
	}
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
SolarSystemTest::~SolarSystemTest()
{
	delete pSphereVB_;
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void SolarSystemTest::onInit()
{
	opengl::frame_buffer::bind_default_frame_buffer();

	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void SolarSystemTest::onExit()
{
	glDisable(GL_DEPTH_TEST);
}
//--------------------------------------------------------------------------------------------------

static float sun_rotation   = 0.0f;
static float earth_rotation = 0.0f;
static float moon_rotation  = 0.0f;
static float earth_orbite   = 0.0f;
static float moon_orbite    = 0.0f;
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

	sun_rotation   = math::rotate(sun_rotation  , 0.0f, math::pi_times_2);
	earth_rotation = math::rotate(earth_rotation, 0.0f, math::pi_times_2);
	moon_rotation  = math::rotate(moon_rotation , 0.0f, math::pi_times_2);
	earth_orbite   = math::rotate(earth_orbite  , 0.0f, math::pi_times_2);
	moon_orbite    = math::rotate(moon_orbite   , 0.0f, math::pi_times_2);
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void SolarSystemTest::draw()
{
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
		shader_.program().sendUniform( "in_World", matWorld[i] );
		shader_.program().sendUniform( "in_WVP", matWorld[i] * matVP);
		pTextures_[i]->bind();
		sphereVA_.draw();
	}
	opengl::texture::unbind();
	shader_.program().end();
}
//--------------------------------------------------------------------------------------------------