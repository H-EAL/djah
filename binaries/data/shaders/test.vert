#version 140

uniform mat4 Projection;
uniform mat4 ModelView;

in vec4 Position;
in vec3 Normal;
in vec2 TexCoord;

invariant gl_Position;

smooth out vec3 n;
smooth out vec3 l;
smooth out vec2 st;

void main()
{	
	vec3 ecPos;
	vec3 aux;
	vec3 lpos = vec3(0,0,50);
	
	n = Normal;
	ecPos = (ModelView * Position).xyz;
	aux = vec3(lpos - ecPos);
	l = normalize(aux);
	
	st = TexCoord;
	gl_Position = Projection * ModelView * Position;
}
