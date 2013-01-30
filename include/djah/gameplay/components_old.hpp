#pragma once

#include "djah/filesystem/stream.hpp"
#include "djah/core/typelist.hpp"
#include "djah/math/vector3.hpp"

#define MAKE_GOC(COMP) static const char* name() { return #COMP; } COMP() { memset(this, 0, sizeof(COMP)); }
#define DESERIALIZER(COMP) static COMP deserialize(const rapidjson::Value &node)

struct position
{
	MAKE_GOC(position);
	djah::math::vector3f value;

	void serialize(djah::filesystem::stream &strm)
	{
		strm << "\"position\": { \"x\": " << value.x << ", \"y\": " << value.y << ", \"z\": " << value.z << " }";
	}

	DESERIALIZER(position)
	{
		position v;
		v.value.x = float(node["x"].GetDouble());
		v.value.y = float(node["y"].GetDouble());
		v.value.z = float(node["z"].GetDouble());
		return v;
	}
};

struct rotation
{
	MAKE_GOC(rotation);
	float angle;
	djah::math::vector3f axis;

	void serialize(djah::filesystem::stream &strm)
	{
		strm << "\"rotation\": { \"angle\": " << angle << " }";
	}

	DESERIALIZER(rotation)
	{
		rotation v;
		v.angle = float(node["angle"].GetDouble());
		return v;
	}
};

struct velocity
{
	MAKE_GOC(velocity);
	djah::math::vector3f value;

	void serialize(djah::filesystem::stream &strm)
	{
		strm << "\"velocity\": { \"x\": " << value.x << ", \"y\": " << value.y << ", \"z\": " << value.z << " }";
	}

	DESERIALIZER(velocity)
	{
		velocity v;
		v.value.x = float(node["x"].GetDouble());
		v.value.y = float(node["y"].GetDouble());
		v.value.z = float(node["z"].GetDouble());
		return v;
	}
};

struct color
{
	MAKE_GOC(color);
	float r,g,b;

	void serialize(djah::filesystem::stream &strm)
	{
		strm << "\"color\": { \"r\": " << r << ", \"g\": " << g << ", \"b\": " << b << " }";
	}

	DESERIALIZER(color)
	{
		color v;
		v.r = float(node["r"].GetDouble());
		v.g = float(node["g"].GetDouble());
		v.b = float(node["b"].GetDouble());
		return v;
	}
};

struct radius
{
	MAKE_GOC(radius);
	float value;

	void serialize(djah::filesystem::stream &strm)
	{
		strm << "\"radius\": { \"value\": " << value << " }";
	}

	DESERIALIZER(radius)
	{
		radius v;
		v.value = float(node["value"].GetDouble());
		return v;
	}
};

struct dimension
{
	MAKE_GOC(dimension);
	float width, height;

	void serialize(djah::filesystem::stream &strm)
	{
		strm << "\"dimension\": { \"width\": " << width << ", \"height\": " << height << " }";
	}

	DESERIALIZER(dimension)
	{
		dimension v;
		v.width = float(node["width"].GetDouble());
		v.height = float(node["height"].GetDouble());
		return v;
	}
};

struct health
{
	MAKE_GOC(health);
	float remaining;

	void serialize(djah::filesystem::stream &strm)
	{
		strm << "\"health\": { \"remaining\": " << remaining << " }";
	}

	DESERIALIZER(health)
	{
		health v;
		v.remaining = float(node["remaining"].GetDouble());
		return v;
	}
};

typedef TYPELIST(position, rotation, velocity, color, dimension, health) default_components_tl;
typedef djah::gameplay::game_object<default_components_tl> game_object_t;