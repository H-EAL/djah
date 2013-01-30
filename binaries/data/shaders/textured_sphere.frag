#version 400

uniform sampler2D in_DiffuseSampler;

in vec3 vs_Normal;

out vec4 FragColor;

void main()
{
	vec3 Normal = normalize(vs_Normal);
	float x2 = Normal.x * Normal.x;
	float z2 = Normal.z * Normal.z;
	float d  = acos( -Normal.x / sqrt(x2 + z2) );
	
	const float PI = 3.14159265359;
	vec2 tc;
	if( d < 0.1 )
		d = 0.0;
	tc.x = d / PI;
	if( Normal.z < 0.0 )
	{
		tc.x = 2.0 - tc.x;
	}
	tc.x = tc.x / 2.0;
	tc.y = acos(-Normal.y) / PI;
	
	FragColor = texture(in_DiffuseSampler, tc);
}