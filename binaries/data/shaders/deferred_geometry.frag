#version 400

uniform sampler2D in_DiffuseSampler;

in vec3 vs_WorldPosition;
in vec3 vs_Normal;
in vec2 vs_TexCoord;

out vec3 WorldPos;
out vec3 Diffuse;
out vec3 Normal;
out vec3 TexCoord;

void main()
{
	WorldPos = vs_WorldPosition;
	Diffuse  = texture(in_DiffuseSampler, vs_TexCoord).xyz;
	Normal   = normalize(vs_Normal);
	TexCoord = vec3(vs_TexCoord, 0);
}