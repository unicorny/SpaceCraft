// simple fragment shader

// 'time' contains seconds since the program was linked.
sampler2D texture;

void main()
{
	gl_FragColor = texture2D(texture, gl_TexCoord[0]);
}
