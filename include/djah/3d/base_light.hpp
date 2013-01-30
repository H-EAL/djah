#ifndef DJAH_3D_BASE_LIGHT_HPP
#define DJAH_3D_BASE_LIGHT_HPP

#include "djah/math/vector3.hpp"

namespace djah { namespace d3d {

	typedef math::vector3f color3f;

	class base_light
	{
	public:
		base_light(const color3f &_color, float _ambientIntensity, float _diffuseIntensity)
			: color_(_color)
			, ambientIntensity_(_ambientIntensity)
			, diffuseIntensity_(_diffuseIntensity)
		{}

		virtual ~base_light() {}

		const color3f& color()            const { return color_;            }
		float          ambientIntensity() const { return ambientIntensity_; }
		float          diffuseIntensity() const { return diffuseIntensity_; }

	protected:
		color3f color_;
		float   ambientIntensity_;
		float   diffuseIntensity_;
	};

} /*d3d*/ } /*djah*/

#endif /* DJAH_3D_BASE_LIGHT_HPP */