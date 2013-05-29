#ifndef TEST_HPP
#define TEST_HPP

#include "djah/dataobject/registry.hpp"
#include "djah/system/device.hpp"
#include "djah/debug/log.hpp"
#include "djah/filesystem/url.hpp"
#include "djah/math.hpp"

using namespace djah;

class test_base
{
public:
	test_base(djah::system::device_sptr pDevice) : pDevice_(pDevice) {}
	virtual ~test_base() {}
	virtual void onInit() = 0;
	virtual void onExit() = 0;
	virtual void update(float dt) = 0;
	virtual void draw() = 0;
	virtual const char* name() const = 0;

protected:
	djah::system::device_sptr pDevice_;
};

inline void testURL()
{
	DJAH_BEGIN_LOG(notification) << "Begin testURL()" << DJAH_END_LOG();

	filesystem::url aURL1 = "file:///D:/Development/Projects/djah/binaries/data/3d/cthulhu.bdae";
	DJAH_BEGIN_LOG(notification) << aURL1.toString() << DJAH_END_LOG();
	DJAH_BEGIN_LOG(notification) << aURL1.dirName() << DJAH_END_LOG();
	DJAH_BEGIN_LOG(notification) << aURL1.fileName() << DJAH_END_LOG();
	DJAH_BEGIN_LOG(notification) << aURL1.extension() << DJAH_END_LOG();
	DJAH_BEGIN_LOG(notification) << aURL1.baseName() << DJAH_END_LOG();

	filesystem::url aURL2 = "data/3d/cthulhu.bdae";
	DJAH_BEGIN_LOG(notification) << aURL2.toString() << DJAH_END_LOG();

	filesystem::url aURL3 = "cthulhu.bdae";
	DJAH_BEGIN_LOG(notification) << aURL3.toString() << DJAH_END_LOG();

	filesystem::url aURL4 = "truc/blah";
	DJAH_BEGIN_LOG(notification) << aURL4.toString() << DJAH_END_LOG();
	DJAH_BEGIN_LOG(notification) << aURL4.dirName() << DJAH_END_LOG();
	DJAH_BEGIN_LOG(notification) << aURL4.fileName() << DJAH_END_LOG();
	DJAH_BEGIN_LOG(notification) << aURL4.extension() << DJAH_END_LOG();
	DJAH_BEGIN_LOG(notification) << aURL4.baseName() << DJAH_END_LOG();

	DJAH_BEGIN_LOG(notification) << "End testURL()" << DJAH_END_LOG();
}

inline void testVec()
{
	//float xy = math::rad_to_deg(std::get<0>(math::oriented_angle(math::vector3f::x_axis, math::vector3f::y_axis)));
	//DJAH_BEGIN_LOG(EWL_LOW) << xy << DJAH_END_LOG();
}

inline void testMat()
{
	math::matrix4x3f A
	(
		14,  9,  3,
		 2, 11, 15,
		 0, 12, 17,
		 5,  2,  3
	);

	math::matrix3x2f B
	(
		12, 25,
		 9, 10,
		 8,  5
	);

	math::matrix2x3i C
	(
		1, 2, 0,
		4, 3, -1
	);

	math::matrix3x2i D
	(
		5, 1,
		2, 3,
		3, 4
	);

	B[2][1] = 4.2f;

	std::cout << A*B << std::endl;
	std::cout << C*D << std::endl;
	std::cout << B[2][1] << std::endl;
}

#endif /* TEST_HPP */