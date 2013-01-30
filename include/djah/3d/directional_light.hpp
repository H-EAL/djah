#ifndef DJAH_3D_DIRECTIONAL_LIGHT_HPP
#define DJAH_3D_DIRECTIONAL_LIGHT_HPP

#include "djah/3d/base_light.hpp"

namespace djah { namespace d3d {

	class directional_light
		: public base_light
	{
	public:
		directional_light(const math::vector3f &_direction, const color3f &_color, float _ambientIntensity, float _diffuseIntensity)
			: base_light(_color, _ambientIntensity, _diffuseIntensity)
			, direction_(_direction)
		{}

		virtual ~directional_light() {}

		const math::vector3f& direction() const { return direction_; }

	protected:
		math::vector3f direction_;
	};

} /*d3d*/ } /*djah*/

#endif /* DJAH_3D_DIRECTIONAL_LIGHT_HPP */