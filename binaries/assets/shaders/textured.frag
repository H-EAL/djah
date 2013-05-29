#version 400

uniform sampler2D in_DiffuseSampler;

in vec2 vs_TexCoord;

out vec4 FragColor;

void main()
{
	FragColor = texture(in_DiffuseSampler, vs_TexCoord);
}