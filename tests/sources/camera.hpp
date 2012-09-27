#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <djah/math.hpp>

using namespace djah;

class Camera
{
public:

	Camera()
	{
		h = 1.8f;
		trans_.setRotation( math::quatf(0.0f,0.0f,0.0f,1.0f) );
	}

	void move(const math::vector3f &newPosition)
	{
		movex(newPosition.x);
		movey(newPosition.y);
		movez(newPosition.z);
	}

	void movex(float xmmod)
	{
		if( xmmod != 0.0f )
		{
			trans_.translate( math::rotate(trans_.rotation(), math::vector3f(xmmod, 0.0f, 0.0f)) );
		}
	}
 
	void movey(float ymmod)
	{
		h += ymmod;
		if( ymmod != 0.0f )
			trans_.translate( math::vector3f(0.0f, -ymmod, 0.0f) );
	}
 
	void movez(float zmmod)
	{
		if( zmmod != 0.0f )
			trans_.translate( math::rotate(trans_.rotation(), math::vector3f(0.0f, 0.0f, zmmod)) );
	}
 
	void rotatex(float xrmod)
	{
		if( xrmod != 0.0f )
		{
			const math::quatf &nrot = math::make_quaternion(math::deg_to_rad(xrmod), math::vector3f(-1.0f, 0.0f, 0.0f));
			trans_.rotate( nrot );
		}
	}
 
	void rotatey(float yrmod)
	{
		if( yrmod != 0.0f )
		{
			const math::quatf &nrot = math::make_quaternion(math::deg_to_rad(yrmod), math::vector3f(0.0f, -1.0f, 0.0f));
			trans_.setRotation( nrot * trans_.rotation() );
		}
	}

	math::vector3f eye() const
	{
		math::vector3f e = trans_.translation();
		e.y = h;
		return e;
	}

	math::vector3f center() const
	{
		return eye() + math::rotate( trans_.rotation(), math::vector3f(0.0f, 0.0f, -1.0f) );
	}

	math::vector3f up() const
	{
		return  math::rotate( trans_.rotation(), math::vector3f(0.0f, 1.0f, 0.0f) );
	}

	math::transformation_f trans_;
	float h;
};

#endif /* CAMERA_HPP */