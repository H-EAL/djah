#version 400

uniform mat4 in_VP;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;
in vec3 Translation;
in vec3 Scale;
//in vec4 Rotation;
in vec3 Color;

out vec3 vs_Normal;
out vec3 vs_Color;
out vec2 vs_TexCoord;

mat3 quaternionToMat3(vec4 quat)
{
	mat3 m = mat3(1,0,0, 0,1,0, 0,0,1);
	
	return m;
}

void main()
{
	//mat3 matRot = quaternionToMat3(Rotation);
	gl_Position = in_VP * vec4((Scale*Position) + Translation, 1);
	vs_Normal = Normal;
	vs_Color = Color;
	vs_TexCoord = TexCoord;
}