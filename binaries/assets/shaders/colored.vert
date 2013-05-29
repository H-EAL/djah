#version 400

uniform mat4 in_WVP;

in vec2 Position;
in vec3 Color;

out vec3 vs_Color;

void main()
{
	gl_Position = in_WVP * vec4(Position,0,1);
	vs_Color = Color;
}