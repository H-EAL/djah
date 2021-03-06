#ifndef DJAH_SOLAR_SYSTEM_TEST_HPP
#define DJAH_SOLAR_SYSTEM_TEST_HPP

#include "test.hpp"
#include "djah/opengl.hpp"
#include "camera.hpp"
#include "djah/system/input/gamepad.hpp"
#include "djah/3d/shader.hpp"
#include "resource_finder.hpp"

class SolarSystemTest
	: public test_base
{
public:
	SolarSystemTest(djah::system::device_sptr pDevice, const djah::system::input::gamepad &g, Camera &cam);
	virtual ~SolarSystemTest();
	virtual void onInit();
	virtual void onExit();
	virtual void update(float dt);
	virtual void draw();
	virtual const char* name() const { return "Solar System"; }

private:
	Camera &cam_;
	const djah::system::input::gamepad &gamepad_;

	djah::math::matrix4f matPerspectiveProj_;

	enum
	{
		ePO_Sun,
		ePO_Earth,
		ePO_Moon,
		ePO_Count
	};

	djah::opengl::vertex_buffer  *pSphereVB_;
	djah::opengl::vertex_array   sphereVA_;
	djah::d3d::texture_sptr		 pTextures_[ePO_Count];
	djah::d3d::shader			  shader_;
};

#endif /* DJAH_SOLAR_SYSTEM_TEST_HPP */