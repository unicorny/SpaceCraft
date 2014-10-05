// simple fragment shader
#version 110
// 'time' contains seconds since the program was linked.

uniform sampler2D texture;
uniform int		cameraAbove;
uniform vec3 transformVertices;

void main()
{
	gl_FragColor = texture2D(texture, gl_TexCoord[0].xy);	
	/*if(cameraAbove == 1 || cameraAbove == 3)
		gl_FragColor.r += 0.4;
	else if(cameraAbove == 2 || cameraAbove == 7)
		gl_FragColor.g += 0.4;
	else if(cameraAbove == 4)
		gl_FragColor.b += 0.4;
	else if(cameraAbove == 0)
		gl_FragColor = vec4(0,0,0,1);
	//*/
	//if(length(transformVertices) > 0.0)
		//gl_FragColor.r += 0.01;
	
}
