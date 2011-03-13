#version 140

in vec3 l;
in vec3 n;
in vec2 st;

out vec4 Color;

uniform sampler2D tex;

void main()
{
	vec3 nn = normalize(n);
	float intensity = max(dot(nn, normalize(l)), 0.0);
	
	Color = texture2D(tex,st) * intensity;
	Color.a = 1;
}