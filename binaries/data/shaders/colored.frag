#version 400

in vec3 vs_Color;

out vec4 FragColor;

void main()
{
	FragColor = vec4(vs_Color,1);
}