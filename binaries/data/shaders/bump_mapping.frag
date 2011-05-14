#version 140

in vec3 lightVec;
in vec3 eyeVec;
in vec2 st;

out vec4 Color;

uniform sampler2D tex;
uniform sampler2D nmap;

void main()
{
	float invRadius = 1.0f;
	
	float distSqr = dot(lightVec, lightVec);
	float att = clamp(1.0 - invRadius * sqrt(distSqr), 0.0, 1.0);
	vec3 lVec = lightVec * inversesqrt(distSqr);

	vec3 vVec = normalize(eyeVec);
	
	vec4 base = texture2D(tex, st);
	
	vec3 bump = normalize( texture2D(nmap, st).xyz * 2.0 - 1.0);

	vec4 vAmbient = vec4(1,1,1,1);
	float diffuse = max( dot(lVec, bump), 0.0 );
	
	vec4 vDiffuse = vec4(1,1,1,1)*diffuse;	

	float specular = pow(clamp(dot(reflect(-lVec, bump), vVec), 0.0, 1.0), 
	                 0.1f );
	
	vec4 vSpecular = vec4(1,1,1,1) * specular;	
	
	Color = ( vAmbient*base + vDiffuse*base + vSpecular) * att;
				
Color = texture2D(tex,st);				
	Color.a = 1.0f;
}