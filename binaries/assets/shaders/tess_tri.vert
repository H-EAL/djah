#version 430

in vec3 Position;
out vec3 vs_Position;

void main()
{
	vs_Position = Position;
}