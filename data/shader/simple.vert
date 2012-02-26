// simple vertex shader
uniform mat4 projection;
uniform mat4 modelview;

void main()
{
	mat4 prj = mat4(1.344, 0, 0, 0,
					0, 1.793, 0, 0,
					0, 0, -1, -1,
					0, 0, -0.20, 0);
	gl_Position    = projection * modelview * gl_Vertex;//projection * modelview * gl_Vertex;
	gl_FrontColor  = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
