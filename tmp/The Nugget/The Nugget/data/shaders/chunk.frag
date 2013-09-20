#version 400

uniform sampler2D in_MatSampler;
uniform sampler2D in_WoodSampler;
//uniform ivec2     in_CameraChunk;
uniform bool	  in_IsBlock;
uniform ivec2 	  in_ChunkCoord;
uniform float 	  in_ChunkProbability;

in vec3 vs_Normal;
in vec2 vs_TexCoord;

flat in ivec2 vs_Coordinates;
flat in float vs_Health;

out vec4  FragColor;
out ivec4 PickColor;

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
	PickColor.x = vs_Coordinates.x + 1;
	PickColor.y = vs_Coordinates.y + 1;
	PickColor.z = in_ChunkCoord.x;
	PickColor.w = in_ChunkCoord.y;
		
	DirectionalLight light;
	light.base.color = vec3(1.0, 1.0, 1.0);
	light.base.ambientIntensity = 0.2;
	light.base.diffuseIntensity = 0.7;
	light.direction = normalize( vec3(0,0,-1) );
	
	//vec4 Color = (in_CameraChunk == in_ChunkCoord) ? vec4(0,1,0,1) : vec4(1,1,1,1);
	FragColor = /*Color */ calcDirectionalLight(light, normalize(vs_Normal));
	
	vec2 TexCoord = vs_TexCoord;
	float type = vs_Health * in_ChunkProbability;
	if( type < 0.25f )
	{
		// SAND
		TexCoord.x += 0.5f;
	}
	else if( type < 0.5f )
	{
		// GRASS
		TexCoord.y += 0.5f;
	}
	else if( type < 0.75f )
	{
		// DIRT
		TexCoord.x += 0.5f;
		TexCoord.y += 0.5f;
	}
	//else
	// ROCK 
	
	if( in_IsBlock )
	{
		FragColor *= texture(in_MatSampler, TexCoord);
		FragColor.a = (vs_Health == 0.0) ? 0.0f : 1.0f;
	}
	else
	{
		FragColor *= texture(in_WoodSampler, vs_TexCoord);
		FragColor.a = 1.0f;
	}
	
}