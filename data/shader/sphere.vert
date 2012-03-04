#version 110

uniform vec3 SphericalCenter;
uniform float theta;
uniform float cameraDistance;
uniform mat4 projection;
uniform mat4 modelview;

uniform sampler2D texture;

void main()
{
	vec4 scaledVertex = vec4(gl_Vertex.xyz*sqrt(cameraDistance), 1.0);
// make it spherical
	//vec4 dir  = scaledVertex-vec4(0.0, 0.0, -0.61794, 1.0);
	vec4 dir  = scaledVertex-vec4(SphericalCenter, 1.0);
	float l = length(dir);
		
//calculate texture coordinates
	gl_TexCoord[0] = vec4(-gl_Vertex.xyz*0.5+0.5, 1.0);//vec4(((-v_texCoord3D.xyz)*0.5+0.5), 1.0);//vec4(-transVertex.x*0.5+0.5, -transVertex.y*0.5+0.5, 0.0, 1.0);
	gl_TexCoord[1] = vec4(-gl_Vertex.x*0.5+0.5, -gl_Vertex.y*0.5+0.5, 0.0, 1.0);
	
	float h = texture2D(texture, gl_TexCoord[0].xy).z;
	vec4 newVertex = scaledVertex + (dir / l) * ((1.0 - l));//+((h*2.0-1.0)/6378.0*6.0));
		
	gl_Position    = projection * modelview * newVertex;

	gl_FrontColor  = gl_Color;
}
