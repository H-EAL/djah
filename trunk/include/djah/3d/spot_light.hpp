#ifndef DJAH_3D_SPOT_LIGHT_HPP
#define DJAH_3D_SPOT_LIGHT_HPP

#include "djah/math/vector3.hpp"
#include "djah/3d/point_light.hpp"

namespace djah { namespace d3d {

	class spot_light
		: public point_light
	{
	public:
		spot_light(const math::vector3f &_position, const math::vector3f &_direction, float _cutoff, const attenuation_t &_attenuation, const color3f &_color, float _ambientIntensity, float _diffuseIntensity)
			: point_light(_position, _attenuation, _color, _ambientIntensity, _diffuseIntensity)
			, direction_(_direction)
			, cutoff_(_cutoff)
		{}

		virtual ~spot_light() {}

		const math::vector3f& direction() const { return direction_; }
		float                 cutoff()    const { return cutoff_;    }

	protected:
		math::vector3f direction_;
		float          cutoff_;
	};

} /*d3d*/ } /*djah*/

#endif /* DJAH_3D_SPOT_LIGHT_HPP */