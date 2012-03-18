#version 110

// simple vertex shader
uniform mat4 projection;
uniform mat4 modelview;

void main()
{
	gl_Position    = projection * modelview * gl_Vertex;//projection * modelview * gl_Vertex;
	gl_FrontColor  = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
