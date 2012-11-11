#ifndef DJAH_SOLAR_SYSTEM_TEST_HPP
#define DJAH_SOLAR_SYSTEM_TEST_HPP

#include "test.hpp"
#include "djah/opengl.hpp"
#include "camera.hpp"
#include "djah/system/input/gamepad.hpp"

class SolarSystemTest
	: public test_base
{
public:
	SolarSystemTest(djah::system::device_ptr pDevice, const djah::system::input::gamepad &g, Camera &cam);
	virtual ~SolarSystemTest();
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
	djah::opengl::vertex_array   *pSphereVA_;
	djah::opengl::texture		 *pTextures_[ePO_Count];
	djah::opengl::shader_program sphereShader_;
};

#endif /* DJAH_SOLAR_SYSTEM_TEST_HPP */