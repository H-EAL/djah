#version 400

uniform sampler2D in_DiffuseSampler;
uniform sampler2D in_NormalMapSampler;
uniform bool in_UseNMap;

in vec2 vs_TexCoord;
in vec3 vs_Normal;
in vec3 vs_Tangent;
in vec3 vs_Binormal;

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

vec3 calcBumpedNormal()
{
    vec3 Tangent   = normalize(vs_Tangent);
    vec3 Normal    = normalize(vs_Normal);
    vec3 Binormal  = normalize(vs_Binormal);
    //Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);
   // vec3 Bitangent = cross(Tangent, Normal);
	
    vec3 BumpMapNormal = texture(in_NormalMapSampler, vs_TexCoord).xyz;
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
	
   // mat3 TBN = mat3(Tangent, Bitangent, Normal);
    mat3 TBN = mat3(Tangent, Binormal, Normal);
    vec3 NewNormal = normalize(TBN * BumpMapNormal);
	
    return NewNormal;
}

void main()
{
	vec3 Normal = in_UseNMap ? calcBumpedNormal() : normalize(vs_Normal);
	
	DirectionalLight light2;
	light2.base.color = vec3(0.89, 0.95, 0.8);
	light2.base.ambientIntensity = 0.3;
	light2.base.diffuseIntensity = 0.8;
	light2.direction = normalize( vec3(0,-1,-1) );
	
	FragColor = texture(in_DiffuseSampler, vs_TexCoord) * calcDirectionalLight(light2, Normal);
}