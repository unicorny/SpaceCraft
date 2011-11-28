#version 120

uniform float time;
/*
 * Both 2D, 3D and 4D texture coordinates are defined, for testing purposes.
 */
varying vec2 v_texCoord2D;
varying vec3 v_texCoord3D;
varying vec4 c_color;

const float PI = 3.1415926535;

void main( void )
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
    vec4 pos = gl_Vertex;
	vec2 tex;
	tex.x = acos(pos.z) / PI;
	if(pos.y > 0.0)
		tex.y = acos(pos.x/(sin(PI*tex.x))) / (2.0*PI);
	else
		tex.y = (PI + acos(pos.x/(sin(PI*tex.x)))) / (2.0*PI);
	v_texCoord2D = gl_MultiTexCoord0.xy * 16.0;
	c_color = gl_Color;

	const int seed =9872;
    v_texCoord3D = normalize(gl_Vertex).xyz + seed; 
	//v_texCoord3D = vec3(gl_MultiTexCoord0.xy, seed);

}

// simple vertex shader

/*void main()
{
	gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor  = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
*/
