#version 400

uniform mat4 in_World;
uniform mat4 in_WVP;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;
in vec3 Tangent;
in vec3 Binormal;

out vec2 vs_TexCoord;
out vec3 vs_Normal;
out vec3 vs_Tangent;
out vec3 vs_Binormal;

void main()
{	
	gl_Position = in_WVP * vec4(Position,1);
	vs_TexCoord = TexCoord;
	vs_Normal   = (in_World * vec4(Normal,0)).xyz;
	vs_Tangent  = (in_World * vec4(Tangent,0)).xyz;
	vs_Binormal = (in_World * vec4(Binormal,0)).xyz;
}
