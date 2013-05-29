#version 400

in vec3 Position;

uniform mat4 in_WVP;

out vec2 vs_TexCoord;

void main()
{
	gl_Position = in_WVP * vec4(Position,1);
	
	const float PI = 3.14159265359;
	vs_TexCoord = asin(Position.xy) / PI + 0.5;
}