#version 400

in vec3 Position;

uniform mat4 in_WVP;

out vec3 vs_Normal;

void main()
{
	gl_Position = in_WVP * vec4(Position, 1);
	vs_Normal = Position;
}