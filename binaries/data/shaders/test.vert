#version 140

uniform mat4 Projection;
uniform mat4 ModelView;

in vec4 Position;
in vec3 Normal;
in vec2 TexCoord;

invariant gl_Position;

smooth out vec3 n;
smooth out vec3 l;
smooth out vec3 l2;
smooth out vec2 st;

void main()
{	
	vec3 ecPos;
	vec3 aux;
	vec3 aux2;
	vec3 lpos = vec3(-100,50,0);
	vec3 lpos2 = vec3(100,50,0);
	
	n = Normal;
	ecPos = (ModelView * Position).xyz;
	aux = vec3(lpos - ecPos);
	aux2 = vec3(lpos2 - ecPos);
	l = normalize(aux);
	l2 = normalize(aux2);
	
	st = TexCoord.st;
	gl_Position = Projection * ModelView * Position;
}
