#version 400

uniform vec3 in_Color;
//uniform bool in_Lit;

//in vec3 vs_Normal;

out vec4 FragColor;


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

vec4 calcLightInternal(BaseLight light, vec3 lightDirection, vec3 normal, float shadowFactor)
{
	vec4 ambientColor = vec4(light.color, 1.0) * light.ambientIntensity;
	float diffuseFactor = dot(normal, -lightDirection);

	vec4 diffuseColor  = vec4(0,0,0,0);
	vec4 specularColor = vec4(0,0,0,0);

	if( diffuseFactor > 0.0 )
	{
		diffuseColor = vec4(light.color, 1.0) * light.diffuseIntensity * diffuseFactor;
	}

	return ambientColor + shadowFactor * (diffuseColor + specularColor);
}

vec4 calcDirectionalLight(DirectionalLight light, vec3 normal)
{
	return calcLightInternal(light.base, light.direction, normal, 1.0);
}

void main()
{
/*
	DirectionalLight light;
	light.base.color = vec3(1.0, 1.0, 1.0);
	light.base.ambientIntensity = 0.5;
	light.base.diffuseIntensity = 0.9;
	light.direction = normalize( vec3(-1,-1,-1) );
*/
	FragColor = vec4(in_Color,1);
	/*
	if( in_Lit )
		FragColor *= calcDirectionalLight(light, normalize(vs_Normal));*/
}