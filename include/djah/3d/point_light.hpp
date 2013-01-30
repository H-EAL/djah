#ifndef DJAH_3D_POINT_LIGHT_HPP
#define DJAH_3D_POINT_LIGHT_HPP

#include "djah/3d/base_light.hpp"

namespace djah { namespace d3d {

	struct attenuation_t
	{
		attenuation_t(float c, float l, float e)
			: constant(c), linear(l), exponential(e) {}
		float constant;
		float linear;
		float exponential;
	};

	class point_light
		: public base_light
	{
	public:
		point_light(const math::vector3f &_position, const attenuation_t &_attenuation, const color3f &_color, float _ambientIntensity, float _diffuseIntensity)
			: base_light(_color, _ambientIntensity, _diffuseIntensity)
			, position_(_position)
			, attenuation_(_attenuation)
		{}

		virtual ~point_light() {}

		const math::vector3f& position()    const { return position_;    }
		const attenuation_t&  attenuation() const { return attenuation_; }

	protected:
		math::vector3f position_;
		attenuation_t  attenuation_;
	};

} /*d3d*/ } /*djah*/

#endif /* DJAH_3D_POINT_LIGHT_HPP */