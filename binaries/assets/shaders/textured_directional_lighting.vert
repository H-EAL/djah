#version 400

// Vertex attributes
in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

// Uniforms
uniform mat4 ModelViewProjection;
uniform mat4 Model;

// Output to the fragment shader
smooth out vec3 vs_Normal;
smooth out vec2 vs_TexCoord;

// Screen space vertex position
invariant gl_Position;

void main()
{	
	gl_Position = ModelViewProjection * vec4(Position, 1.0);
	vs_TexCoord = TexCoord;
}
