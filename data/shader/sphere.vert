#version 110

uniform float patchScaling;
uniform mat4 projection;
uniform mat4 modelview;
uniform vec3 translate;
uniform float MAX_HEIGHT_IN_PERCENT;
uniform float MIN_HEIGHT_IN_PERCENT;
uniform vec2  textureCoordsParam; //contain id.xy (norm)

uniform sampler2D texture;

void main()
{
	vec4 scaledVertex = vec4(gl_Vertex.xy*(patchScaling*1.001)+translate.xy, gl_Vertex.z, 1.0);
			
	//calculate texture coordinates
	float scale = 1.0;
	if(textureCoordsParam.x != 0.0 && textureCoordsParam.y != 0.0) scale = 0.5;

	gl_TexCoord[0] = vec4((-gl_Vertex.xyz*0.5+0.5)*scale+vec3(textureCoordsParam, 0.0), 1.0);//vec4(((-v_texCoord3D.xyz)*0.5+0.5), 1.0);//vec4(-transVertex.x*0.5+0.5, -transVertex.y*0.5+0.5, 0.0, 1.0);
	/*if(gl_TexCoord[0].x < 0.01) gl_TexCoord[0].x = 0.0;
	if(gl_TexCoord[0].x > 0.99) gl_TexCoord[0].x = 1.0;
	if(gl_TexCoord[0].y < 0.01) gl_TexCoord[0].y = 0.0;
	if(gl_TexCoord[0].y > 0.99) gl_TexCoord[0].y = 1.0;
	*/
	
	//gl_TexCoord[1] = vec4((-gl_Vertex.xy*0.5+0.5)*scale+posOffset, 0.0, 1.0);
	
	float h = texture2D(texture, gl_TexCoord[0].xy).w;
	//h = 0.0;
	vec4 newVertex = vec4(normalize(scaledVertex.xyz)*(1.0+h*MAX_HEIGHT_IN_PERCENT), 1.0);
		
	gl_Position    = projection * modelview * newVertex;
//    gl_Position    = gl_ModelViewProjectionMatrix  * newVertex;

	gl_FrontColor  = gl_Color;
}
