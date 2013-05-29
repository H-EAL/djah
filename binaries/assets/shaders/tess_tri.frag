#version 430

uniform sampler2D in_DiffuseSampler;
uniform float in_HeightMax = 10.0f;
uniform vec3 in_LightDir = vec3(-1,-1,-1);

in vec3 te_Normal;
in vec2 te_TexCoord;
in float te_Height;
out vec3 FragColor;


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

struct Attenuation
{
	float constant;
	float linear;
	float exponential;
};

struct PointLight
{
	BaseLight base;
	vec3 position;
	Attenuation attenuation;
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
/*
vec4 calcPointLight(PointLight light, vec3 normal)
{
	vec3 lightDirection = vs_WorldPos - light.position;
	float dist = length(lightDirection);
	lightDirection = normalize(lightDirection);
	float shadowFactor = 1.0;
	
	vec4 color = calcLightInternal(light.base, lightDirection, normal, shadowFactor);
	float attenuation = light.attenuation.constant + light.attenuation.linear * dist + light.attenuation.exponential * dist * dist;
	
	return shadowFactor * color / attenuation;
}
*/

void main()
{
	vec3 c[5];
	c[0] = vec3(0,0,1);
	c[1] = vec3(1,1,0);
	c[2] = vec3(0,1,0);
	c[3] = vec3(0.5,0.5,0.5);
	c[4] = vec3(1,1,1);
	
	vec3 up = vec3(0,1,0);
	
	DirectionalLight light;
	light.base.color = vec3(1.0, 1.0, 1.0);
	light.base.ambientIntensity = 0.2;
	light.base.diffuseIntensity = 1.0;
	light.direction = normalize( in_LightDir );
	
	vec3 Normal = normalize(te_Normal);
	
	vec3 Color;
	if( te_Height <= 0.0f )
		Color = c[0];
	else if( dot(Normal, up) < 0.5f )
		Color = c[3];
	else if( te_Height > 0.9f * in_HeightMax )
		Color = c[4];
	else if( te_Height < 0.1f * in_HeightMax )
		Color = c[1];
	else
		Color = c[2];
	
	FragColor = texture2D(in_DiffuseSampler, te_TexCoord).rgb * Color * calcDirectionalLight(light, Normal).xyz;
	//FragColor = Normal;
}