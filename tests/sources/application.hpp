#ifndef DJAH_TESTS_APPLICATION_HPP
#define DJAH_TESTS_APPLICATION_HPP

#include <djah/math.hpp>

#include <djah/filesystem/memory_stream.hpp>

#include <djah/3d/text.hpp>

#include <djah/opengl.hpp>

#include <djah/system/input/mouse.hpp>
#include <djah/system/input/keyboard.hpp>
#include <djah/system/input/gamepad.hpp>

#include <djah/dataobject/global_registry.hpp>

#include <djah/application_base.hpp>

#include "camera.hpp"
#include "shadow_test.hpp"
#include "basic_test.hpp"
#include "deferred_shading_test.hpp"
#include "solar_system_test.hpp"
#include "bump_mapping_test.hpp"
#include "font_test.hpp"


class application
	: public djah::application_base
{
public:

	explicit application(int w = 1280, int h = 720);

private:

	virtual void initImpl();
	virtual void runImpl();
	virtual void exitImpl();

	void draw2D();

	djah::dataobject::default_registry::data_object_ptr screenCfgDo_;

	djah::math::matrix4f matOrthoProj_;
	Camera cam;

	djah::system::input::mouse		mouse_;
	djah::system::input::keyboard	keyboard_;
	djah::system::input::gamepad	gamepad_;

	djah::d3d::text fps_str_;
	djah::d3d::text mouse_pos_;
	djah::d3d::text cam_pos_;
	djah::d3d::text test_name_;

	ShadowTest			 *pShadowTest_;
	BasicTest			 *pBasicTest_;
	DeferredShadingTest  *pDeferredShadingTest_;
	SolarSystemTest		 *pSolarSystemTest_;
	BumpMappingTest		 *pBumpMappingTest_;
	FontTest			 *pFontTest_;
	test_base			 *pCurrentTest_;
};

DJAH_ENTRY_POINT(application);

#endif /* DJAH_TESTS_APPLICATION_HPP */