#version 400

uniform sampler2D in_DiffuseSampler;
uniform vec3 in_Color;

in vec3 vs_WorldPos;
in vec3 vs_Normal;
flat in int vs_PetalID;

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


void main()
{
	vec3 C[6];
	C[0] = vec3(1,0,0);
	C[1] = vec3(0,1,0);
	C[2] = vec3(0,0,1);
	C[3] = vec3(1,1,0);
	C[4] = vec3(1,0,1);
	C[5] = vec3(0,1,1);
	
	DirectionalLight light;
	light.base.color = vec3(1.0, 1.0, 1.0);
	light.base.ambientIntensity = 0.2;
	light.base.diffuseIntensity = 1.0;
	light.direction = normalize( vec3(0,0,-1) );
	
	PointLight ptLight;
	ptLight.base.color = vec3(1.0, 1.0, 0.8);
	ptLight.base.ambientIntensity = 0.1;
	ptLight.base.diffuseIntensity = 1.0;
	ptLight.position = vec3(0,0,0);
	ptLight.attenuation.constant = 0.1;
	ptLight.attenuation.linear   = 0.2;
	ptLight.attenuation.exponential = 0.0001;
	
	//vec3 Color = texture(in_DiffuseSampler, vs_TexCoord).xyz;
	//vec4 LightColor = calcPointLight(ptLight, normalize(vs_Normal));
	vec4 LightColor = calcDirectionalLight(light, normalize(vs_Normal));
   	FragColor = vec4(in_Color,1);// * LightColor;
}