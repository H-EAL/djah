#version 400

in vec3 Position;
in vec2 TexCoord;

uniform mat4 in_WorldViewProjection;

out vec2 vs_TexCoord;

void main()
{
	gl_Position = in_WorldViewProjection * vec4(Position, 1.0);
	vs_TexCoord = TexCoord;
}