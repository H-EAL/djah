#version 400

uniform sampler2D in_MatSampler;
uniform ivec2     in_CameraChunk;
uniform bool	  in_IsBlock;
uniform ivec2 	  in_ChunkCoord;

in vec3 vs_Normal;
in vec2 vs_TexCoord;

flat in ivec2 vs_Coordinates;
flat in float vs_Health;

out vec4  FragColor;
out ivec3 PickColor;

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
	ivec3 coords;
	coords.x = ((vs_Coordinates.x+1) << 16) | (vs_Coordinates.y+1);
	coords.y = in_ChunkCoord.x;
	coords.z = in_ChunkCoord.y;
	PickColor = coords;
		
	DirectionalLight light;
	light.base.color = vec3(1.0, 1.0, 1.0);
	light.base.ambientIntensity = 0.2;
	light.base.diffuseIntensity = 0.7;
	light.direction = normalize( vec3(0,0,-1) );
	
	vec4 Color = (in_CameraChunk == in_ChunkCoord) ? vec4(0,1,0,1) : vec4(1,1,1,1);
		
	FragColor = Color * texture(in_MatSampler, vs_TexCoord) * calcDirectionalLight(light, normalize(vs_Normal));
	FragColor.a = (in_IsBlock && vs_Health < 0.5f) ? 0.0f : 1.0f;
		
}