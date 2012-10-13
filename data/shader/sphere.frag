// simple fragment shader
#version 110
// 'time' contains seconds since the program was linked.

uniform sampler2D texture;
uniform int		cameraAbove;

void main()
{
	gl_FragColor = texture2D(texture, gl_TexCoord[0].xy);	
	if(cameraAbove > 0)
		gl_FragColor.r += 0.5;
}
