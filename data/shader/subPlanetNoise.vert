
varying vec3 v_texCoord3D;
varying vec2 v_texCoord2D;
varying vec4 c_color;

uniform vec3 SphericalCenter;
uniform float theta;



// simple vertex shader
void main()
{
	vec4 scaledVertex = vec4(gl_Vertex.xyz*sqrt(1.0-cos(theta)), 1.0);
	// make it spherical
	vec4 dir = scaledVertex-vec4(SphericalCenter, 1.0);
	
	gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;	

//calculate texture coordinates
	v_texCoord3D = vec4(gl_TextureMatrix[0] * normalize(dir)).xyz;

	gl_FrontColor  = gl_Color;	
}
