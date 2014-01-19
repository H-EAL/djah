#version 400

uniform mat4 in_WVP;
uniform bool in_Wireframe;

in vec3 Position;
in vec3 Normal;

out vec3 vs_WorldPos;
out vec3 vs_Normal;
flat out int vs_PetalID;

void main()
{	
	vs_PetalID = gl_VertexID/48;
	const float scale = 0.5f*sqrt(21);
	vec4 displacement[3];
	displacement[0] = scale*vec4( 1.0f, 0.0f,            -0.5f,  0.86602540378f);
	displacement[1] = scale*vec4( 0.5f, 0.86602540378f,  -1.0f,  0.0f);
	displacement[2] = scale*vec4(-0.5f, 0.86602540378f,  -0.5f, -0.86602540378f);
	
	int id = gl_InstanceID;
	
	// Real positive root of the equation:
	// id <= (ring-1) * (6 * ring) / 2
	float root = -0.5f + sqrt(9 + 12 * id) / 6.0f;
	
	// Rounding errors workaround
	float ceiledRoot = ceil(root);
	float diff = ceiledRoot - root;
	float ONE_MINUS_EPSILON = 1.0f - 0.0001f;
	// Id of the ring the floret is in [0->]
	int ring = int((diff > ONE_MINUS_EPSILON) ? ceiledRoot-1 : ceiledRoot);
	
	int ringId = id - 3 * ring * (ring - 1) - 1;
	int section = ringId / ring;
	int sectionId = ringId % ring;
	
	vec4 Pos = vec4(Position, 1);
	
	if( section < 3 )
	{
		Pos.xy += ring * displacement[section].xy;
		Pos.xy += sectionId * displacement[section].zw;
	}
	else
	{
		Pos.xy -= ring * displacement[section%3].xy;
		Pos.xy -= sectionId * displacement[section%3].zw;
	}
	
	if( in_Wireframe )
	{
		Pos.xyz *= 1.001f;
	}
	
	gl_Position = in_WVP * Pos;
	
	vs_WorldPos = Pos.xyz;
	vs_Normal = Normal;
}