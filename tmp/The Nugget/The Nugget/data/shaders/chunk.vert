#version 400

uniform mat4  in_VP;
uniform ivec2 in_ChunkCoord;
uniform float in_CellSize = 1.18;
uniform ivec2 in_CellsPerChunk;
uniform bool  in_IsBlock;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

in float Health;

out vec3 vs_Normal;
out vec2 vs_TexCoord;
out vec3 vs_Color;

flat out ivec2 vs_Coordinates;
flat out ivec2 vs_ChunkCoord;
flat out float vs_Health;

void main()
{	
	ivec2 CellCoord = ivec2(gl_InstanceID % in_CellsPerChunk.x, gl_InstanceID / in_CellsPerChunk.x);
	vec3 chunkTranslation = vec3(in_ChunkCoord * in_CellsPerChunk * in_CellSize + vec2(in_CellSize/2.0), 0);
	vec3 cellTranslation  = vec3(CellCoord * in_CellSize, in_IsBlock ? -0.2 : 0.0);
	
	gl_Position = in_VP * vec4(Position + chunkTranslation + cellTranslation, 1);
	
	vs_Normal = Normal;
	vs_TexCoord = TexCoord;
	vs_Coordinates = in_IsBlock ? ivec2(CellCoord) : ivec2(-1);
	
	vs_Health = Health;
}