#version 400

in vec3 Position;

uniform mat4 in_WVP;

void main()
{
	gl_Position = in_WVP * vec4(Position,1);
}