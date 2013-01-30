#version 400

uniform mat4 in_WVP;

in vec2 Position;
in vec2 TexCoord;

out vec2 vs_TexCoord;

void main()
{
	gl_Position = in_WVP * vec4(Position,0,1);
	vs_TexCoord = TexCoord;
}