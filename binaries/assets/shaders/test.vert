#version 400

uniform mat4 in_WorldViewProjection;
uniform mat4 in_World;
uniform mat4 in_LightWVP;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

out vec3 vs_Normal;
out vec2 vs_TexCoord;
out vec3 vs_WorldPos;
out vec4 vs_LightSpacePos;

const mat4 bias = mat4(0.5, 0.0, 0.0, 0.0,
                       0.0, 0.5, 0.0, 0.0,
                       0.0, 0.0, 0.5, 0.0,
                       0.5, 0.5, 0.5, 1.0);

invariant gl_Position;

void main()
{	
	gl_Position      = in_WorldViewProjection * vec4(Position, 1.0);
	vs_Normal        = (in_World * vec4(Normal, 0.0)).xyz;
	vs_TexCoord      = TexCoord;
	vs_WorldPos      = (in_World * vec4(Position, 1.0)).xyz;
	vs_LightSpacePos = bias * in_LightWVP * vec4(Position, 1.0);
}
