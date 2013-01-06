#version 110
// simple fragment shader

// 'time' contains seconds since the program was linked.
uniform sampler2D texture;

void main()
{

	gl_FragColor = texture2D(texture, gl_TexCoord[0].xy);
}
