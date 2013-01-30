struct BaseLight
{
	vec3  color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight
{
	BaseLight base;
	vec3 direction;
};

struct PointLight
{
	BaseLight base;
	vec3 position;
};

struct SpotLight
{
	PointLight 	base;
	vec3 		direction;
	float 		cutoff;
};