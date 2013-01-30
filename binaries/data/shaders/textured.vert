#version 400

in vec3 Position;
in vec2 TexCoord;

uniform mat4 in_WVP;

out vec2 vs_TexCoord;

void main()
{
	gl_Position = in_WVP * vec4(Position, 1);
	vs_TexCoord = TexCoord;
}