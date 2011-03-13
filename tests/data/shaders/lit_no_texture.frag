#version 140

in vec3 l;
in vec3 n;
in vec2 st;

out vec4 Color;

uniform vec4 in_Color;

void main()
{
	float intensity = max(dot(normalize(n), normalize(l)), 0.0);
	
	Color = in_Color*intensity;
	Color.a = 1;
}