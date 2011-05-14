#version 140

uniform mat4 Projection;
uniform mat4 ModelView;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;
//in vec3 Tangent;

invariant gl_Position;

smooth out vec3 lightVec;
smooth out vec3 eyeVec;
smooth out vec2 st;

void main()
{	
	vec3 lpos = vec3(-100,50,0);
	vec3 vVertex  = (ModelView * vec4(Position,1.0f)).xyz;
	vec3 tmpVec  = lpos - vVertex ;
	
	vec3 Tangent = vec3(1,0,0);
	
	vec3 n = normalize(Normal);
	vec3 t = normalize(Tangent);
	vec3 b = cross(n,t);
	
	lightVec.x = dot(tmpVec , t);
	lightVec.y = dot(tmpVec , b);
	lightVec.z = dot(tmpVec , n);
	
	tmpVec = -vVertex ;
	eyeVec.x = dot(tmpVec , t);
	eyeVec.y = dot(tmpVec , b);
	eyeVec.z = dot(tmpVec , n);
	
	st = TexCoord;
	gl_Position = Projection * ModelView * vec4(Position,1.0f);
}
