// simple fragment shader
#version 110
// 'time' contains seconds since the program was linked.

uniform sampler2D texture;
uniform sampler2D texture2;

void main()
{
//Default
/*	float alpha = texture2D(texture, gl_TexCoord[0].xy).w;
	if(alpha > 0.5) alpha = 1.0;
	alpha = 1.0;
	gl_FragColor = mix(texture2D(texture2, gl_TexCoord[3].xy),
					   texture2D(texture, gl_TexCoord[0].xy),
					   alpha);
*/
	gl_FragColor = texture2D(texture, gl_TexCoord[0].xy);
	
}
