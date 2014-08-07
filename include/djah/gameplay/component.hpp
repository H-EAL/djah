#ifndef DJAH_GAMEPLAY_COMPONENTS_COMPONENT_HPP
#define DJAH_GAMEPLAY_COMPONENTS_COMPONENT_HPP

#include "rapidjson/document.h"
#include "djah/core/macros.hpp"
#include "djah/gameplay/json_serializer.hpp"

//--------------------------------------------------------------------------------------------------
#define DESERIALIZE_ATTRIBUTE(Attr)  resources::json_deserialize(#Attr, Attr, node)

#define load_attributes_1(A1) DESERIALIZE_ATTRIBUTE(A1)
#define load_attributes_2(A1,A2) DESERIALIZE_ATTRIBUTE(A1); load_attributes_1(A2)
#define load_attributes_3(A1,A2,A3) DESERIALIZE_ATTRIBUTE(A1); load_attributes_2(A2,A3)
#define load_attributes_4(A1,A2,A3,A4) DESERIALIZE_ATTRIBUTE(A1); load_attributes_3(A2,A3,A4)
#define load_attributes_5(A1,A2,A3,A4,A5) DESERIALIZE_ATTRIBUTE(A1); load_attributes_4(A2,A3,A4,A5)
#define load_attributes_6(A1,A2,A3,A4,A5,A6) DESERIALIZE_ATTRIBUTE(A1); load_attributes_5(A2,A3,A4,A5,A6)
#define load_attributes_7(A1,A2,A3,A4,A5,A6,A7) DESERIALIZE_ATTRIBUTE(A1); load_attributes_6(A2,A3,A4,A5,A6,A7)
#define load_attributes_8(A1,A2,A3,A4,A5,A6,A7,A8) DESERIALIZE_ATTRIBUTE(A1); load_attributes_7(A2,A3,A4,A5,A6,A7,A8)
#define load_attributes_9(A1,A2,A3,A4,A5,A6,A7,A8,A9) DESERIALIZE_ATTRIBUTE(A1); load_attributes_8(A2,A3,A4,A5,A6,A7,A8,A9)

#define load_attributes(...) VA_ARGS_MACRO(load_attributes_, __VA_ARGS__)
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
#define SERIALIZE_ATTRIBUTE(Attr) resources::json_serialize(document, componentNode, #Attr, Attr)

#define save_attributes_1(A1) SERIALIZE_ATTRIBUTE(A1)
#define save_attributes_2(A1,A2) SERIALIZE_ATTRIBUTE(A1); save_attributes_1(A2)
#define save_attributes_3(A1,A2,A3) SERIALIZE_ATTRIBUTE(A1); save_attributes_2(A2,A3)
#define save_attributes_4(A1,A2,A3,A4) SERIALIZE_ATTRIBUTE(A1); save_attributes_3(A2,A3,A4)
#define save_attributes_5(A1,A2,A3,A4,A5) SERIALIZE_ATTRIBUTE(A1); save_attributes_4(A2,A3,A4,A5)
#define save_attributes_6(A1,A2,A3,A4,A5,A6) SERIALIZE_ATTRIBUTE(A1); save_attributes_5(A2,A3,A4,A5,A6)
#define save_attributes_7(A1,A2,A3,A4,A5,A6,A7) SERIALIZE_ATTRIBUTE(A1); save_attributes_6(A2,A3,A4,A5,A6,A7)
#define save_attributes_8(A1,A2,A3,A4,A5,A6,A7,A8) SERIALIZE_ATTRIBUTE(A1); save_attributes_7(A2,A3,A4,A5,A6,A7,A8)
#define save_attributes_9(A1,A2,A3,A4,A5,A6,A7,A8,A9) SERIALIZE_ATTRIBUTE(A1); save_attributes_8(A2,A3,A4,A5,A6,A7,A8,A9)

#define save_attributes(...) VA_ARGS_MACRO(save_attributes_, __VA_ARGS__)
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
#define MAKE_COMPONENT_2(COMP, COUNT) \
	static const unsigned int NB_COMP = COUNT;\
	static const char* name() { return #COMP; }\
	COMP() {}\
	COMP(const rapidjson::Value &node);\
	void serialize(rapidjson::Document &document, rapidjson::Value &componentNode) const
//--------------------------------------------------------------------------------------------------
#define MAKE_COMPONENT_1(COMP) MAKE_COMPONENT_2(COMP, 100)
//--------------------------------------------------------------------------------------------------
#define MAKE_COMPONENT(...) VA_ARGS_MACRO(MAKE_COMPONENT_, __VA_ARGS__)
//--------------------------------------------------------------------------------------------------

#endif /* DJAH_GAMEPLAY_COMPONENTS_COMPONENT_HPP */