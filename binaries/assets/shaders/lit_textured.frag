#version 400

uniform bool      in_UsePointLight = false;
uniform sampler2D in_DiffuseSampler;
uniform sampler2D in_SpecularSampler;
uniform vec3      in_EyeWorldPos;
uniform vec2	  in_UVScale = vec2(1,1);
uniform vec2	  in_UVOffset = vec2(1,1);

in vec3 vs_WorldPos;
in vec3 vs_Normal;
in vec2 vs_TexCoord;

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
		
		vec3 VertexToEye = normalize(in_EyeWorldPos - vs_WorldPos);
        vec3 LightReflect = normalize(reflect(lightDirection, normal));
        float SpecularFactor = dot(VertexToEye, LightReflect);
        SpecularFactor = pow(SpecularFactor, 2);
        if (SpecularFactor > 0)
		{
			float SpecularIntensity = texture(in_SpecularSampler, in_UVOffset + vs_TexCoord * in_UVScale).x;
            specularColor = vec4(light.color, 1.0f) * SpecularIntensity * SpecularFactor;
        }
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
	DirectionalLight light;
	light.base.color = vec3(1.0, 1.0, 1.0);
	light.base.ambientIntensity = 0.2;
	light.base.diffuseIntensity = 1.0;
	light.direction = normalize( vec3(0,-1,0) );
	
	PointLight ptLight;
	ptLight.base.color = vec3(1.0, 1.0, 0.8);
	ptLight.base.ambientIntensity = 0.1;
	ptLight.base.diffuseIntensity = 1.0;
	ptLight.position = vec3(0,3,2);
	ptLight.attenuation.constant = 0.1;
	ptLight.attenuation.linear   = 0.2;
	ptLight.attenuation.exponential = 0.0001;
	
	vec3 Color = texture(in_DiffuseSampler, in_UVOffset + vs_TexCoord * in_UVScale).xyz;
	vec4 LightColor = in_UsePointLight
		? calcPointLight(ptLight, normalize(vs_Normal))
		: calcDirectionalLight(light, normalize(vs_Normal));
   	FragColor = vec4(Color,1) * LightColor;
}