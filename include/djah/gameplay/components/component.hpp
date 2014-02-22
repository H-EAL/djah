#ifndef DJAH_GAMEPLAY_COMPONENTS_COMPONENT_HPP
#define DJAH_GAMEPLAY_COMPONENTS_COMPONENT_HPP

#include "rapidjson/document.h"
#include "djah/core/macros.hpp"
#include "djah/filesystem/stream.hpp"
#include "djah/gameplay/json_serializer.hpp"

//--------------------------------------------------------------------------------------------------
#define read_attribute(Attr)  resources::json_deserialize(#Attr, Attr, node)

#define load_attributes_1(A1) read_attribute(A1)
#define load_attributes_2(A1,A2) read_attribute(A1); load_attributes_1(A2)
#define load_attributes_3(A1,A2,A3) read_attribute(A1); load_attributes_2(A2,A3)
#define load_attributes_4(A1,A2,A3,A4) read_attribute(A1); load_attributes_3(A2,A3,A4)
#define load_attributes_5(A1,A2,A3,A4,A5) read_attribute(A1); load_attributes_4(A2,A3,A4,A5)
#define load_attributes_6(A1,A2,A3,A4,A5,A6) read_attribute(A1); load_attributes_5(A2,A3,A4,A5,A6)
#define load_attributes_7(A1,A2,A3,A4,A5,A6,A7) read_attribute(A1); load_attributes_6(A2,A3,A4,A5,A6,A7)
#define load_attributes_8(A1,A2,A3,A4,A5,A6,A7,A8) read_attribute(A1); load_attributes_7(A2,A3,A4,A5,A6,A7,A8)
#define load_attributes_9(A1,A2,A3,A4,A5,A6,A7,A8,A9) read_attribute(A1); load_attributes_8(A2,A3,A4,A5,A6,A7,A8,A9)

#define load_attributes(...) VA_ARGS_MACRO(load_attributes_, __VA_ARGS__)
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
#define write_attribute(Attr) resources::json_serialize(#Attr, Attr, node)

#define save_attributes_1(A1) write_attribute(A1)
#define save_attributes_2(A1,A2) write_attribute(A1); save_attributes_1(A2)
#define save_attributes_3(A1,A2,A3) write_attribute(A1); save_attributes_2(A2,A3)
#define save_attributes_4(A1,A2,A3,A4) write_attribute(A1); save_attributes_3(A2,A3,A4)
#define save_attributes_5(A1,A2,A3,A4,A5) write_attribute(A1); save_attributes_4(A2,A3,A4,A5)
#define save_attributes_6(A1,A2,A3,A4,A5,A6) write_attribute(A1); save_attributes_5(A2,A3,A4,A5,A6)
#define save_attributes_7(A1,A2,A3,A4,A5,A6,A7) write_attribute(A1); save_attributes_6(A2,A3,A4,A5,A6,A7)
#define save_attributes_8(A1,A2,A3,A4,A5,A6,A7,A8) write_attribute(A1); save_attributes_7(A2,A3,A4,A5,A6,A7,A8)
#define save_attributes_9(A1,A2,A3,A4,A5,A6,A7,A8,A9) write_attribute(A1); save_attributes_8(A2,A3,A4,A5,A6,A7,A8,A9)

#define save_attributes(...) rapidjson::Value node(name()); VA_ARGS_MACRO(save_attributes_, __VA_ARGS__)
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
#define MAKE_COMPONENT(COMP) \
	static const char* name() { return #COMP; }\
	COMP() {}\
	COMP(const rapidjson::Value &node);\
	void serialize(rapidjson::Document &doc) const;
//--------------------------------------------------------------------------------------------------


#define serializable(type, data) serializable_data<type> data;

template<typename T>
struct serializable_data
{
	serializable_data(const std::string &name)
	{
		resources::json_deserialize(data, node, name);
	}

	T data;
};

#endif /* DJAH_GAMEPLAY_COMPONENTS_COMPONENT_HPP */