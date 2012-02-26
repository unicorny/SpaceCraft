
#version 120
uniform mat4 projection;

varying vec3 v_texCoord3D;

const float PI = 3.1415926535;

// simple vertex shader

void main()
{
	gl_Position = projection * gl_Vertex;
	float lon =  -PI      - (gl_Vertex.x*0.5+0.5)* (2.0*PI);
	float lat = (-PI*0.5) - (gl_Vertex.y*0.5+0.5)*      PI ;
    float r = cos(lat);
	//if(lon >= PI) lon -= 0.0001;
	//if(lat >= PI) lon -= 0.0001;
	v_texCoord3D = vec3(r*cos(lon), sin(lat), r*sin(lon));
	
	gl_FrontColor  = gl_Color;	
}
