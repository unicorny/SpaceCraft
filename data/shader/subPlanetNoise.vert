
#version 120
uniform mat4 projection;
uniform mat4 texture;

varying vec3 v_texCoord3D;

uniform float patchScaling;
uniform vec3  translate;


// simple vertex shader
void main()
{
	vec4 scaledVertex = vec4(gl_Vertex.xy*patchScaling+translate.xy, gl_Vertex.z, 1.0);
	// make it spherical
	vec4 dir = vec4(normalize(scaledVertex.xyz), 1.0);
	
	gl_Position    = projection * gl_Vertex;	
//	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

//calculate texture coordinates
	v_texCoord3D = vec4(texture * dir).xyz;

	gl_FrontColor  = gl_Color;	
}
