#version 110
// simple fragment shader

// 'time' contains seconds since the program was linked.
uniform sampler2D texture;
uniform float SEA_LEVEL_IN_METRES;
uniform int		cameraAbove;

struct GradientColor
{
	float pos;
	vec4 color;
};

vec4 LinearInterpColor (vec4 color0, vec4 color1, float alpha)
{
	return (color1 * alpha) + (color0 * (1.0 - alpha));
}

int clampi(int x, int minVal, int maxVal) 
{
  	if(x < minVal) return minVal;
	else if (x > maxVal) return maxVal;
  	else return x;
}

vec4 gradientColor(float value, GradientColor points[10], int pointsCount)
{
  // Find the first element in the control point array that has a value
  // larger than the output value from the source module.
  int indexPos;
  for (indexPos = 0; indexPos < pointsCount; indexPos++) {
    if (value < points[indexPos].pos) {
      break;
    }
  }

  // Find the two nearest control points so that we can map their values
  // onto a quadratic curve.
  ivec2 index = ivec2(clampi(indexPos - 1, 0, pointsCount - 1),
					  clampi(indexPos    , 0, pointsCount - 1));

  // If some control points are missing (which occurs if the output value from
  // the source module is greater than the largest value or less than the
  // smallest value of the control point array), get the value of the nearest
  // control point and exit now.
  if (index.x == index.y) 
    return points[index.y].color;

  // Compute the alpha value used for linear interpolation.
  vec2 inputv = vec2(points[index.x].pos, points[index.y].pos);
  float alpha = (value - inputv.x) / (inputv.y - inputv.x);

  vec4 color0 = points[index.x].color;
  vec4 color1 = points[index.y].color;

  // Now perform the linear interpolation given the alpha value.
  return LinearInterpColor (color0, color1, alpha);
}

float unpackHeight(vec3 color)
{   
	return (color.r+color.g*256.0 + (color.b*256.0*256.0))/(256.0*256.0);
}

vec3 packHeight(float height) 
{ 
	float b = floor(height * 256.0);
	float g = floor(height * 65536.0) - (b*256.0);
	float r = (floor(height * 16777216.0) - (b*65536.0)) - (g*256.0);
	return vec3(r, g, b)/ 256.0;
}

void main()
{
	
	GradientColor gradient[10];
	
	gradient[0] =  GradientColor(-2.0 			+ SEA_LEVEL_IN_METRES, vec4(0.0,     0.0,     0.0,     1.0));
	gradient[1] =  GradientColor(-0.03125 	  	+ SEA_LEVEL_IN_METRES, vec4(0.02353, 0.22745, 0.49804, 1.0));
	gradient[2] =  GradientColor(-0.0001220703 	+ SEA_LEVEL_IN_METRES, vec4(0.05490, 0.43922, 0.75294, 1.0));
	gradient[3] =  GradientColor( 0.0 			+ SEA_LEVEL_IN_METRES, vec4(0.27451, 0.47059, 0.23529, 1.0));
	gradient[4] =  GradientColor( 0.125 		+ SEA_LEVEL_IN_METRES, vec4(0.43137, 0.54902, 0.29412, 1.0));
	gradient[5] =  GradientColor( 0.25 			+ SEA_LEVEL_IN_METRES, vec4(0.62745, 0.54902, 0.43529, 1.0));
	gradient[6] =  GradientColor( 0.375 		+ SEA_LEVEL_IN_METRES, vec4(0.72157, 0.63921, 0.55294, 1.0));
	gradient[7] =  GradientColor( 0.5 			+ SEA_LEVEL_IN_METRES, vec4(1.0));
	gradient[8] =  GradientColor( 0.75 		 	+ SEA_LEVEL_IN_METRES, vec4(0.5,     1.0,     1.0,     1.0));
	gradient[9] =  GradientColor( 2.0 			+ SEA_LEVEL_IN_METRES, vec4(0.0,     0.0,     1.0,     1.0));

	float n = unpackHeight(texture2D(texture, gl_TexCoord[0].xy).xyz*2.0-1.0);
	gl_FragColor = gradientColor(n, gradient, 10);//vec4(0.5 + 0.5*vec3(n, n, n), 1.0);
	if(cameraAbove > 0)
		gl_FragColor.g += 0.1;
	gl_FragColor.w = n * 0.5 + 0.5;
	
	//gl_FragColor = texture2D(texture, gl_TexCoord[0].xy);
	
}
