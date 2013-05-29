#include "djah/gameplay/components.hpp"
#include "djah/debug/assertion.hpp"
#include "djah/gameplay/json_serializer.hpp"

namespace djah { namespace gameplay { namespace components {

	//----------------------------------------------------------------------------------------------
	void transform::serialize(djah::filesystem::stream &strm) const
	{
		strm << position;
		strm << orientation;
		strm << scale;
	}
	//----------------------------------------------------------------------------------------------
	void transform::deserialize(const rapidjson::Value &node)
	{
		position = json_serializer<math::vector3f>::deserialize(node, "position");
		orientation = json_serializer<math::vector3f>::deserialize(node, "orientation");
		scale = json_serializer<math::vector3f>::deserialize(node, "scale");
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


	//----------------------------------------------------------------------------------------------
	void static_mesh::serialize(djah::filesystem::stream &strm) const
	{
		strm << file;
	}
	//----------------------------------------------------------------------------------------------
	void static_mesh::deserialize(const rapidjson::Value &node)
	{
		file = json_serializer<std::string>::deserialize(node, "file");
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void texture::serialize(djah::filesystem::stream &strm) const
	{
		strm << file;
	}
	//----------------------------------------------------------------------------------------------
	void texture::deserialize(const rapidjson::Value &node)
	{
		file = json_serializer<std::string>::deserialize(node, "file");
	}
	//----------------------------------------------------------------------------------------------

} /*components*/ } /*gameplay*/ } /*djah*/