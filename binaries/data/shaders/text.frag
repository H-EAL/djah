#version 400

uniform sampler2D in_DiffuseSampler;

in vec2 vs_TexCoord;

out vec4 FragColor;

void main()
{
	float p = texture(in_DiffuseSampler, vs_TexCoord).r;
	
	/**
	if( p == 0 )
		FragColor = vec4(1,0,1,1);
	else
	/**/
		FragColor = vec4(p,p,p,p);		
}