uniform vec3 SphericalCenter;
uniform float theta;
uniform float theta2;
uniform vec2 rotation;

uniform sampler2D texture;

varying vec3 v_texCoord3D;

const float PI = 3.1415926535;
vec3 sphereToKartesian(vec3 spherical)
{
	vec3 cartesian;

	//x = p cos (0) sin(oI)
	cartesian.x = spherical.x*cos(spherical.y)*sin(spherical.z);

	//z = p sin (0) sin(oI )
	cartesian.z = spherical.x*sin(spherical.y)*sin(spherical.z);

	//y = p cos (oI )
	cartesian.y = spherical.x*cos(spherical.z);

	return cartesian;
}

vec3 cartesianToSphere(vec3 cartesian)
{
	vec3 spherical;

	//p = Wurzel(x^2 + y^2 + z^2)
	spherical.x = length(cartesian);

	//O = tan^-1(z/x)
	spherical.y = atan(cartesian.z, cartesian.x);

	//oI = cos^-1(y/p)
	spherical.z = acos(cartesian.y/spherical.x);

	return spherical;
}

void main()
{
	vec4 scaledVertex = vec4(gl_Vertex.xyz*sqrt(1.0-cos(theta)), 1.0);
	vec4 diffScale = vec4(gl_Vertex.xyz*sqrt(cos(1.0-theta/theta2)), 1.0);
// make it spherical
	vec4 dir  = scaledVertex-vec4(SphericalCenter, 1.0);
	vec4 nonScaledDir = gl_Vertex-vec4(0.0, 0.0, -1.0, 1.0);
	
	float l = length(dir);
	float lNS = length(nonScaledDir.xyz);
		
//calculate texture coordinates
	v_texCoord3D = normalize(vec4(gl_TextureMatrix[0] * normalize(nonScaledDir)).xyz)*lNS;
	gl_TexCoord[0] = vec4(-gl_Vertex.xyz*0.5+0.5, 1.0);//vec4(((-v_texCoord3D.xyz)*0.5+0.5), 1.0);//vec4(-transVertex.x*0.5+0.5, -transVertex.y*0.5+0.5, 0.0, 1.0);
	vec3 texCoord2 = vec4(gl_TextureMatrix[1] * normalize(nonScaledDir)).xyz;
	vec3 spherical = cartesianToSphere(v_texCoord3D);
	vec4 transVertex = (vec4(gl_Vertex.xy*0.5+0.5, 0.0, 1.0));
	
	gl_TexCoord[1] = vec4((-(texCoord2+SphericalCenter))*0.5+0.5, 1.0);
	gl_TexCoord[2] = vec4(spherical.yz/PI, 0.0, 1.0);
	gl_TexCoord[3] = vec4(-gl_Vertex.x*0.5+0.5, -gl_Vertex.y*0.5+0.5, 0.0, 1.0);
	
	
	float h = texture2D(texture, gl_TexCoord[0].xy).z;
	vec4 newVertex = scaledVertex + (dir / l) * ((1.0 - l)+((h*2.0-1.0)/6378.0*6.0));
		
	gl_Position    = gl_ModelViewProjectionMatrix * newVertex;

	gl_FrontColor  = gl_Color;
}
