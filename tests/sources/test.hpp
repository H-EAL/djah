#ifndef TEST_HPP
#define TEST_HPP

void testURL()
{
	DJAH_BEGIN_LOG(EWL_NOTIFICATION) << "Begin testURL()" << DJAH_END_LOG();

	filesystem::url aURL1 = "file:///D:/Development/Projects/djah/binaries/data/3d/cthulhu.bdae";
	DJAH_BEGIN_LOG(EWL_LOW) << aURL1.toString() << DJAH_END_LOG();
	DJAH_BEGIN_LOG(EWL_LOW) << aURL1.dirName() << DJAH_END_LOG();
	DJAH_BEGIN_LOG(EWL_LOW) << aURL1.fileName() << DJAH_END_LOG();
	DJAH_BEGIN_LOG(EWL_LOW) << aURL1.extension() << DJAH_END_LOG();
	DJAH_BEGIN_LOG(EWL_LOW) << aURL1.baseName() << DJAH_END_LOG();

	filesystem::url aURL2 = "data/3d/cthulhu.bdae";
	DJAH_BEGIN_LOG(EWL_LOW) << aURL2.toString() << DJAH_END_LOG();

	filesystem::url aURL3 = "cthulhu.bdae";
	DJAH_BEGIN_LOG(EWL_LOW) << aURL3.toString() << DJAH_END_LOG();

	filesystem::url aURL4 = "truc/blah";
	DJAH_BEGIN_LOG(EWL_LOW) << aURL4.toString() << DJAH_END_LOG();
	DJAH_BEGIN_LOG(EWL_LOW) << aURL4.dirName() << DJAH_END_LOG();
	DJAH_BEGIN_LOG(EWL_LOW) << aURL4.fileName() << DJAH_END_LOG();
	DJAH_BEGIN_LOG(EWL_LOW) << aURL4.extension() << DJAH_END_LOG();
	DJAH_BEGIN_LOG(EWL_LOW) << aURL4.baseName() << DJAH_END_LOG();

	DJAH_BEGIN_LOG(EWL_NOTIFICATION) << "End testURL()" << DJAH_END_LOG();
}

void testVec()
{
	float xy = math::rad_to_deg(math::oriented_angle(math::vector3f::x_axis, math::vector3f::y_axis));
	DJAH_BEGIN_LOG(EWL_LOW) << xy << DJAH_END_LOG();
}

void testMat()
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