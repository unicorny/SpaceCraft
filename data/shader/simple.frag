#version 110
// simple fragment shader

// 'time' contains seconds since the program was linked.
uniform sampler2D texture;
uniform int      textureLoaded;

void main()
{
	if(textureLoaded == 0)
		gl_FragColor = gl_Color;
	else
		gl_FragColor = texture2D(texture, gl_TexCoord[0].xy);
}
