#version 140

in vec3 Position;
//in vec3 Color;
//in vec3 Normal;

uniform mat4 MVP;
uniform mat4 TR;

smooth out vec4 c;

invariant gl_Position;

void main()
{
	vec4 p = vec4(Position,1.0f);
	c = vec4(Position, 1.0f);
	
	gl_Position = MVP * TR * p;
}