#version 400

//uniform mat4 in_World;
uniform mat4 in_WVP;

in vec3 Position;
//in vec3 Normal;

//out vec3 vs_Normal;

void main()
{
	gl_Position = in_WVP * vec4(Position,1);
	//vs_Normal = (in_World * vec4(Normal,0)).xyz;
}