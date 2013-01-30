#version 400
 
in vec2 vs_TexCoord;

uniform sampler2D in_Sampler;

out vec4 FragColor;

void main()
{
	FragColor = vec4(1.0, 0.0, 0.0, 0.0);//texture2D(in_Sampler, vs_TexCoord);
}