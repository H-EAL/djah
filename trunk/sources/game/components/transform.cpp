#include "djah/game/components/transform.hpp"

namespace djah { namespace game { namespace components {

	//----------------------------------------------------------------------------------------------
	transform::transform(const rapidjson::Value &node)
	{
		load_attributes
		(
			  position
			, orientation
			, scale
		);
	}
	//----------------------------------------------------------------------------------------------
	void transform::serialize(rapidjson::Document &document, rapidjson::Value &componentNode) const
	{
		save_attributes
		(
			  position
			, orientation
			, scale
		);
	}
	//----------------------------------------------------------------------------------------------
	math::matrix4f transform::toMatrix4() const
	{
		float matScaleTrans[4*4] =
		{
			scale.x,		0.0f,	        0.0f,           0.0f,
			0.0f,			scale.y,		0.0f,           0.0f,
			0.0f,			0.0f,	        scale.z,		0.0f,
			position.x,		position.y,		position.z,		1.0f
		};

		return math::quat_to_matrix4(orientation) * math::matrix4f(matScaleTrans);
	}

} /*components*/ } /*game*/ } /*djah*/