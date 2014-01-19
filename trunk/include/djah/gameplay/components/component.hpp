#ifndef DJAH_GAMEPLAY_COMPONENTS_COMPONENT_HPP
#define DJAH_GAMEPLAY_COMPONENTS_COMPONENT_HPP

#include "rapidjson/document.h"
#include "djah/filesystem/stream.hpp"
#include "djah/gameplay/json_serializer.hpp"

#define JSON_READ(AttrName) resources::json_deserialize(AttrName, node, #AttrName)
#define JSON_WRITE(AttrName) strm << AttrName

#define MAKE_COMPONENT(COMP) \
	static const char* name() { return #COMP; }\
	COMP() {}\
	COMP(const rapidjson::Value &node);\
	void serialize(djah::filesystem::stream &pStream) const;\

#endif /* DJAH_GAMEPLAY_COMPONENTS_COMPONENT_HPP */