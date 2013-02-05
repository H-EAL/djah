#version 400

uniform mat4 in_VP;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;
in vec3 Translation;
in vec3 Scale;
in vec4 Rotation;
in vec3 Color;

out vec3 vs_Normal;
out vec3 vs_Color;
out vec2 vs_TexCoord;


void main()
{
	gl_Position = in_VP * vec4((Scale*Position) + Translation, 1);
	vs_Normal = Normal;
	vs_Color = Color;
	vs_TexCoord = TexCoord;
}