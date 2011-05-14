#version 140

in vec3 l;
in vec3 l2;
in vec3 n;
in vec2 st;

out vec4 Color;

uniform sampler2D tex;

void main()
{
	vec3 nn = normalize(n);
	float intensity = max(dot(nn, normalize(l)), 0.0);
	float intensity2 = max(dot(nn, normalize(l2)), 0.0);
	
	Color = texture2D(tex,st) * min(intensity + intensity2, 1.0f);
	Color.a = 1;
}