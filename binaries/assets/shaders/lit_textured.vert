#version 400

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform mat4 in_World;
uniform mat4 in_WVP;

out vec3 vs_WorldPos;
out vec3 vs_Normal;
out vec2 vs_TexCoord;

void main()
{
	gl_Position = in_WVP * vec4(Position, 1.0);
	vs_WorldPos = (in_World * vec4(Position, 1.0)).xyz;
	vs_Normal   = (in_World * vec4(Normal, 0.0)).xyz;
	vs_TexCoord = TexCoord;
}