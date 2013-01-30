#version 400

//uniform sampler2D in_PositionSampler;
uniform sampler2D in_DiffuseSampler;
uniform sampler2D in_NormalSampler;
//uniform sampler2D in_TexCoordSampler;

uniform vec2 in_ScreenSize;

out vec4 FragColor;

vec2 CalcTexCoord()
{
   return gl_FragCoord.xy / in_ScreenSize;
}


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
	vec2 TexCoord = CalcTexCoord();
   	//vec3 WorldPos = texture(in_PositionSampler, TexCoord).xyz;
   	vec3 Color    = texture(in_DiffuseSampler, TexCoord).xyz;
   	vec3 Normal   = texture(in_NormalSampler, TexCoord).xyz;
   	Normal        = normalize(Normal);
	
	
	DirectionalLight light2;
	light2.base.color = vec3(0.89, 0.95, 0.8);
	light2.base.ambientIntensity = 0.1;
	light2.base.diffuseIntensity = 0.6;
	light2.direction = normalize( vec3(-1,-1,0) );
	
   	FragColor = vec4(Color, 1.0) * calcDirectionalLight(light2, Normal);
}