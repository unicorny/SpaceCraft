
#version 120
uniform mat4 projection;

// simple vertex shader
void main()
{
	gl_Position    = projection * gl_Vertex;	
	gl_FrontColor  = gl_Color;	
	gl_TexCoord[0] = gl_Vertex;//gl_MultiTexCoord0;
}
