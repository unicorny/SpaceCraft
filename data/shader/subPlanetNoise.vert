
#version 120
uniform mat4 projection;
uniform mat4 texture;

varying vec3 v_texCoord3D;

uniform vec3 SphericalCenter;
uniform float theta;


// simple vertex shader
void main()
{
	vec4 scaledVertex = vec4(gl_Vertex.xyz*sqrt(1.0-cos(theta)), 1.0);
	// make it spherical
	vec4 dir = scaledVertex-vec4(SphericalCenter, 1.0);
	
	gl_Position    = projection * gl_Vertex;	

//calculate texture coordinates
	v_texCoord3D = vec4(texture * normalize(dir)).xyz;

	gl_FrontColor  = gl_Color;	
}
