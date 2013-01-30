#include "djah/gameplay/components.hpp"
#include "djah/debug/assertion.hpp"
#include "djah/gameplay/json_serializer.hpp"

namespace djah { namespace gameplay { namespace components {

	//----------------------------------------------------------------------------------------------
	void position::serialize(djah::filesystem::stream &strm) const
	{
		strm << value;
	}
	//----------------------------------------------------------------------------------------------
	void position::deserialize(const rapidjson::Value &node)
	{
		value = json_serializer<math::vector3f>::deserialize(node, "value");
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void orientation::serialize(djah::filesystem::stream &strm) const
	{
		strm << value;
	}
	//----------------------------------------------------------------------------------------------
	void orientation::deserialize(const rapidjson::Value &node)
	{
		value = json_serializer<math::quatf>::deserialize(node, "value");
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void fov::serialize(djah::filesystem::stream &strm) const
	{
		strm << horizontal;
		strm << vertical;
	}
	//----------------------------------------------------------------------------------------------
	void fov::deserialize(const rapidjson::Value &node)
	{
		horizontal = json_serializer<float>::deserialize(node, "horizontal");
		vertical = json_serializer<float>::deserialize(node, "vertical");
	}
	//----------------------------------------------------------------------------------------------

} /*components*/ } /*gameplay*/ } /*djah*/