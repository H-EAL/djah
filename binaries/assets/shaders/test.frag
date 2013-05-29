#version 400

uniform float     in_TexRepeat;
uniform sampler2D in_DiffuseSampler;
uniform sampler2D in_ShadowMapSampler;
uniform vec3      in_SpotPosition;
uniform vec3      in_SpotDirection;

in vec3 vs_Normal;
in vec2 vs_TexCoord;
in vec3 vs_WorldPos;
in vec4 vs_LightSpacePos;

smooth out vec4 FragColor;

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

struct SpotLight
{
	PointLight 	base;
	vec3 		direction;
	float 		cutoff;
};


float calcShadowFactor(vec4 lightSpacePos)
{
	float epsilon = 0.001;
	
	float shadowFactor = 1.0;
	if( lightSpacePos.w > 0.0 )
	{
		vec3 projCoords = lightSpacePos.xyz;
		projCoords.z += -0.51;
		projCoords   /= lightSpacePos.w;
		if( projCoords.s >= 0.0 && projCoords.t >= 0.0 && projCoords.s < 1.0 && projCoords.t < 1.0 )
		{
			float depth = texture(in_ShadowMapSampler, projCoords.st).r;
			float z = 0.5 * projCoords.z + 0.5;
			if( depth < (z + epsilon) )
			{
				shadowFactor = 0.5;
			}
		}
	}
	return shadowFactor;
}

vec4 calcLightInternal(BaseLight light, vec3 lightDirection, vec3 normal, float shadowFactor)
{
	vec4 ambientColor = vec4(light.color, 1.0) * light.ambientIntensity;
	float diffuseFactor = dot(normal, -lightDirection);
	
	vec4 diffuseColor  = vec4(0,0,0,0);
	vec4 specularColor = vec4(0,0,0,0);
	
	if( diffuseFactor > 0.0 )
	{
		diffuseColor      = vec4(light.color, 1.0) * light.diffuseIntensity * diffuseFactor;
		
		/*
		vec3 vertexToEye  = normalize(in_EyeWorldPos - vs_WorldPos);
		vec3 lightReflect = normalize( reflect(lightDirection, normal) );
		float specularFactor = dot(vertexToEye, lightReflect);
		specularFactor = pow(specularFactor, in_SpecularPower);
		if( specularFactor > 0.0 )
		{
			specularColor = vec4(light.color, 1.0) * in_materialSpecularIntensity * specularFactor;
		}
		*/
	}
	
	return ambientColor + shadowFactor * (diffuseColor + specularColor);
}

vec4 calcDirectionalLight(DirectionalLight light, vec3 normal)
{
	return calcLightInternal(light.base, light.direction, normal, 1.0);
}

vec4 calcPointLight(PointLight light, vec3 normal, vec4 lightSpacePos)
{
	vec3 lightDirection = vs_WorldPos - light.position;
	float dist = length(lightDirection);
	lightDirection = normalize(lightDirection);
	float shadowFactor = calcShadowFactor(lightSpacePos);
	
	vec4 color = calcLightInternal(light.base, lightDirection, normal, shadowFactor);
	float attenuation = light.attenuation.constant + light.attenuation.linear * dist + light.attenuation.exponential * dist * dist;
	
	return shadowFactor * color / attenuation;
}

vec4 calcSpotLight(SpotLight light, vec3 normal, vec4 lightSpacePos)
{
	vec3 lightToPixel = normalize(vs_WorldPos - light.base.position);
	float spotFactor = dot(lightToPixel, light.direction);
	
	vec4 color = vec4(0,0,0,0);
	if( spotFactor > light.cutoff )
	{
		color = calcPointLight(light.base, normal, lightSpacePos);
		color = color * (1.0 - (1.0 - spotFactor) * 1.0/(1.0 - light.cutoff));
	}
	return color;
}

void main()
{
	SpotLight light;
	light.base.base.color = vec3(1,1,1);
	light.base.base.ambientIntensity = 0.2;
	light.base.base.diffuseIntensity = 1.0;
	light.base.position = in_SpotPosition;
	light.base.attenuation.constant = 0.1;
	light.base.attenuation.linear   = 0.2;
	light.base.attenuation.exponential = 0.0001;
	//light.direction = normalize( vec3(0.0, 0.0, 0.0) - in_SpotPosition );
	light.direction = in_SpotDirection;
	light.cutoff = 0.86602540378;
	//light.cutoff = 0.9659;
	
	DirectionalLight light2;
	light2.base.color = vec3(0.89, 0.95, 0.8);
	light2.base.ambientIntensity = 0.01;
	light2.base.diffuseIntensity = 0.2;
	light2.direction = normalize( vec3(0,-1,0) );

	vec3 normal = normalize(vs_Normal);
	vec4 color = calcSpotLight( light, normal, vs_LightSpacePos );
	//color += calcDirectionalLight(light2, normal);
	FragColor = texture(in_DiffuseSampler, vs_TexCoord * in_TexRepeat) * color;
}