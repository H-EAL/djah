#include "tess_test.hpp"
#include "djah/types.hpp"
#include "resource_finder.hpp"
#include "djah/3d/primitives.hpp"
#include "djah/resources/media_manager.hpp"
#include "djah/core/randomizer.hpp"
#include "djah/core/noise.hpp"
#include "camera.hpp"

using namespace djah;
using namespace d3d;

//--------------------------------------------------------------------------------------------------
TesselationTest::TesselationTest(djah::system::device_sptr pDevice, djah::system::input::keyboard &kb, Camera &cam)
	: test_base(pDevice)
	, cam_(cam)
	, kb_(kb)
	, pVB_(nullptr)
	, shader_("tess_tri")
	, pGO("main_camera")
{
	std::vector<game_object_t*> vgo;
	vgo.push_back(&pGO);

	game_object_t::serializer_t::deserialize(vgo);

	const float w = static_cast<float>(pDevice_->config().width);
	const float h = static_cast<float>(pDevice_->config().height);
	matPerspectiveProj_ = math::make_perspective_projection(60.0f, w/h, 0.1f, 1000.f);

	const int W = 10;
	const int H = 10;
	math::vector3f vertices[W*H];
	for(int i = 0; i < W; ++i)
		for(int j = 0; j < H; ++j)
			vertices[i+j*W] = math::vector3f(float(i), 0.0f, float(j));

	pVB_ = new opengl::vertex_buffer(sizeof(vertices), opengl::eBU_StaticDraw);
	pVB_->write(vertices);
	opengl::vertex_format vertexFormat;
	vertexFormat << opengl::format::position<3,float>();
	vertArr_.addVertexBuffer(pVB_, vertexFormat);
	vertArr_.setVertexCount(W*H);
	vertArr_.init(shader_.program());

	pTexture_ = d3d::texture_manager::get().find("detail0.jpg");
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
TesselationTest::~TesselationTest()
{
	delete pVB_;
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void TesselationTest::onInit()
{
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	opengl::frame_buffer::bind_default_frame_buffer();

	glLineWidth(3.0f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	DJAH_GLOBAL_NOTIFICATION()
		<< "Max supported patch vertices "
		<< opengl::capabilities::value_of(GL_MAX_PATCH_VERTICES)
		<< DJAH_END_LOG();

	glPatchParameteri(GL_PATCH_VERTICES, 1);
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void TesselationTest::onExit()
{
	glDisable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
//--------------------------------------------------------------------------------------------------


static int innerLevel = 64;
static int outerLevel = 64;
static float s_p = 0.5f;
static float s_zoom = 75.0f;
static int s_octaves = 7;
static float s_height_max = 10.0f;
static math::vector3f s_light_dir = math::vector3f(-1.0f, 0.0f, 0.0f);
//--------------------------------------------------------------------------------------------------
void TesselationTest::update(float dt)
{
	if( kb_.isKeyDown(system::input::eKC_O) )
		innerLevel = math::clamp(innerLevel-1, 1, 1000);
	if( kb_.isKeyDown(system::input::eKC_P) )
		innerLevel = math::clamp(innerLevel+1, 1, 1000);
	if( kb_.isKeyDown(system::input::eKC_K) )
		outerLevel = math::clamp(outerLevel-1, 1, 1000);
	if( kb_.isKeyDown(system::input::eKC_L) )
		outerLevel = math::clamp(outerLevel+1, 1, 1000);

	if( kb_.pressed(system::input::eKC_Z) )
		s_p = math::clamp(s_p-0.1f, 0.0f, 1000.0f);
	if( kb_.pressed(system::input::eKC_X) )
		s_p = math::clamp(s_p+0.1f, 0.0f, 1000.0f);

	if( kb_.isKeyDown(system::input::eKC_C) )
		s_zoom = math::clamp(s_zoom-1.0f, 0.0f, 1000.0f);
	if( kb_.isKeyDown(system::input::eKC_V) )
		s_zoom = math::clamp(s_zoom+1.0f, 0.0f, 1000.0f);

	if( kb_.pressed(system::input::eKC_B) )
		s_octaves = math::clamp(s_octaves-1, 0, 1000);
	if( kb_.pressed(system::input::eKC_N) )
		s_octaves = math::clamp(s_octaves+1, 0, 1000);

	if( kb_.pressed(system::input::eKC_F) )
		s_height_max = math::clamp(s_height_max-1.0f, 1.0f, 1000.0f);
	if( kb_.pressed(system::input::eKC_G) )
		s_height_max = math::clamp(s_height_max+1.0f, 1.0f, 1000.0f);

	static float angle = 0.0f;

	s_light_dir = math::vector3f(-cos(math::deg_to_rad(angle)), -sin(math::deg_to_rad(angle)), 0.0f);
	
	static float speed = 50.0f;
	angle += speed * dt;

	angle = math::rotate(angle, 0.0f, 360.0f);
}
//--------------------------------------------------------------------------------------------------


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
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void TesselationTest::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const math::matrix4f &viewMat = math::make_look_at(cam_.eye(), cam_.center(), cam_.up());
	const math::matrix4f &matWorld = math::make_translation(-5.0f*64.0f, 0.0f, -5.0f*64.0f);
	const math::matrix4f &matWVP = matWorld * viewMat * matPerspectiveProj_;

	shader_.program().begin();
	shader_.program().sendUniform("in_WVP", matWVP);
	shader_.program().sendUniform("in_TessLevelInner", innerLevel);
	shader_.program().sendUniform("in_TessLevelOuter", outerLevel);
	shader_.program().sendUniform("in_P", s_p);
	shader_.program().sendUniform("in_Zoom", s_zoom);
	shader_.program().sendUniform("in_Octaves", s_octaves);
	shader_.program().sendUniform("in_HeightMax", s_height_max);
	shader_.program().sendUniform("in_LightDir", s_light_dir);
	pTexture_->bind();
	vertArr_.draw(GL_PATCHES);
	pTexture_->unbind();
	shader_.program().end();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(&matPerspectiveProj_[0][0]);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(&viewMat[0][0]);
	drawAxis();
}
//--------------------------------------------------------------------------------------------------