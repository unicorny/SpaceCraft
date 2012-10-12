//
// GLSL textureless classic 3D noise "cnoise",
// with an RSL-style periodic variant "pnoise".
// Author:  Stefan Gustavson (stefan.gustavson@liu.se)
// Version: 2011-04-03
//
// Many thanks to Ian McEwan of Ashima Arts for the
// ideas for permutation and gradient selection.
//
// This code is licensed under the terms of the MIT license:
//
// Copyright (c) 2011 Stefan Gustavson (stefan.gustavson@liu.se)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

//#version 120
#version 120

uniform sampler2D permTexture;
uniform sampler2D texture0; // continents with badlands 

varying vec3 v_texCoord3D;

uniform float CONTINENT_FREQUENCY;
uniform float CONTINENT_LACUNARITY;
uniform float MOUNTAIN_LACUNARITY;
uniform float HILLS_LACUNARITY;
uniform float PLAINS_LACUNARITY;
uniform float BADLANDS_LACUNARITY;
uniform float MOUNTAIN_GLACIATION;
uniform float SEA_LEVEL;
uniform float SHELF_LEVEL;
uniform float HILLS_AMOUNT;
uniform float MOUNTAINS_AMOUNT;
uniform float BADLANDS_AMOUNT;
uniform float RIVER_DEPTH;
uniform float SEA_LEVEL_IN_METRES;
uniform float CONTINENT_HEIGHT_SCALE;
uniform float TERRAIN_OFFSET;
uniform float MOUNTAINS_TWIST;
uniform float HILLS_TWIST;
uniform float BADLANDS_TWIST;


float f_lacunarity;
float f_persistence;


//libnoise helper
float SCurve3 (float a) {return (a * a * (3.0 - 2.0 * a));}
float LinearInterp (float n0, float n1, float a) {return ((1.0 - a) * n0) + (a * n1);}

int clampi(int x, int minVal, int maxVal) 
{
  	if(x < minVal) return minVal;
	else if (x > maxVal) return maxVal;
  	else return x;
}

float clampf(float x, float minVal, float maxVal) 
{
  	if(x < minVal) return minVal;
	else if (x > maxVal) return maxVal;
  	else return x;
}

float CubicInterp (vec4 n, float a)
{//      0 1 2 3
//vec4 = x y z w
  float p = (n.w - n.z) - (n.x - n.y);
  float q = (n.x - n.y) - p;
  float r = n.z - n.x;
  float s = n.y;
  return p * a * a * a + q * a * a + r * a + s;
}

/*
 * To create offsets of one texel and one half texel in the
 * texture lookup, we need to know the texture image size.
 */
#define ONE 0.00390625
#define ONEHALF 0.001953125
// The numbers above are 1/256 and 0.5/256, change accordingly
// if you change the code to use another texture size.

/*
 * The interpolation function. This could be a 1D texture lookup
 * to get some more speed, but it's not the main part of the algorithm.
 */
float fade(float t) {
  // return t*t*(3.0-2.0*t); // Old fade, yields discontinuous second derivative
  return t*t*t*(t*(t*6.0-15.0)+10.0); // Improved fade, yields C2-continuous noise
}

// noise helper
vec3 fade(vec3 t) {return t*t*t*(t*(t*6.0-15.0)+10.0);}
  /*
 * 3D classic noise. Slower, but a lot more useful than 2D noise, with texture lookup
 */
float noise(vec3 P)
{
  vec3 Pi = ONE*floor(P)+ONEHALF; // Integer part, scaled so +1 moves one texel
                                  // and offset 1/2 texel to sample texel centers
  vec3 Pf = fract(P);     // Fractional part for interpolation

  // Noise contributions from (x=0, y=0), z=0 and z=1
  float perm00 = texture2D(permTexture, Pi.xy).a ;
  vec3  grad000 = texture2D(permTexture, vec2(perm00, Pi.z)).rgb * 4.0 - 1.0;
  float n000 = dot(grad000, Pf);
  vec3  grad001 = texture2D(permTexture, vec2(perm00, Pi.z + ONE)).rgb * 4.0 - 1.0;
  float n001 = dot(grad001, Pf - vec3(0.0, 0.0, 1.0));

  // Noise contributions from (x=0, y=1), z=0 and z=1
  float perm01 = texture2D(permTexture, Pi.xy + vec2(0.0, ONE)).a ;
  vec3  grad010 = texture2D(permTexture, vec2(perm01, Pi.z)).rgb * 4.0 - 1.0;
  float n010 = dot(grad010, Pf - vec3(0.0, 1.0, 0.0));
  vec3  grad011 = texture2D(permTexture, vec2(perm01, Pi.z + ONE)).rgb * 4.0 - 1.0;
  float n011 = dot(grad011, Pf - vec3(0.0, 1.0, 1.0));

  // Noise contributions from (x=1, y=0), z=0 and z=1
  float perm10 = texture2D(permTexture, Pi.xy + vec2(ONE, 0.0)).a ;
  vec3  grad100 = texture2D(permTexture, vec2(perm10, Pi.z)).rgb * 4.0 - 1.0;
  float n100 = dot(grad100, Pf - vec3(1.0, 0.0, 0.0));
  vec3  grad101 = texture2D(permTexture, vec2(perm10, Pi.z + ONE)).rgb * 4.0 - 1.0;
  float n101 = dot(grad101, Pf - vec3(1.0, 0.0, 1.0));

  // Noise contributions from (x=1, y=1), z=0 and z=1
  float perm11 = texture2D(permTexture, Pi.xy + vec2(ONE, ONE)).a ;
  vec3  grad110 = texture2D(permTexture, vec2(perm11, Pi.z)).rgb * 4.0 - 1.0;
  float n110 = dot(grad110, Pf - vec3(1.0, 1.0, 0.0));
  vec3  grad111 = texture2D(permTexture, vec2(perm11, Pi.z + ONE)).rgb * 4.0 - 1.0;
  float n111 = dot(grad111, Pf - vec3(1.0, 1.0, 1.0));

  // Blend contributions along x
  vec4 n_x = mix(vec4(n000, n001, n010, n011),
                 vec4(n100, n101, n110, n111), fade(Pf.x));

  // Blend contributions along y
  vec2 n_xy = mix(n_x.xy, n_x.zw, fade(Pf.y));

  // Blend contributions along z
  float n_xyz = mix(n_xy.x, n_xy.y, fade(Pf.z));

  // We're done, return the final noise value.
  return n_xyz;
}

float sOctaveNoise(vec3 p, float frequenzy, int octaveCount)
{
	float value = 0.0;
	float curPersistence = 1.0;
//	cnoise(p*frequenzy);
	for(int curOctave = 0; curOctave < octaveCount; curOctave++)
	{
		value += noise(p*frequenzy) * curPersistence;
		p *= f_lacunarity;
		curPersistence *= f_persistence;
	}
	return value;
}

float sridged(vec3 p, float frequency, int octaveCount)
{
	float signal = 0.0;
	float value = 0.0;
	float weight = 1.0;

	float spectralWeightFrequenz = 1.0;

    // These parameters should be user-defined; they may be exposed in a
    // future version of libnoise.
	float offset = 1.0;
	float gain = 2.0;

	p *= frequency;

	for(int curOctave = 0; curOctave < octaveCount; curOctave++)
	{
		signal = noise(p);

		// Make the ridges.
		signal = abs(signal);
		signal = offset - signal;
		
		// Square the signal to increase the sharpness of the ridges.
		signal *= signal;	

		// The weighting from the previous octave is applied to the signal.
	    // Larger values have higher weights, producing sharp points along the
	    // ridges.
	    signal *= weight;

		// Weight successive contributions by the previous signal.
    	weight = signal * gain;
    	if (weight > 1.0) weight = 1.0;
    	if (weight < 0.0) weight = 0.0;
   
    	// Add the signal to the output value.
    	value += (signal * pow(spectralWeightFrequenz, -1.0));

    	// Go to the next octave.
    	p *= f_lacunarity;
		spectralWeightFrequenz *= f_lacunarity;
	} 
	return value*1.2-1.0;
}

float sbillow(vec3 p, float frequency, int octaveCount)
{
	float value = 0.0;
	float signal = 0.0;
	float curPersistence = 1.0;

	p *= frequency;
//	cnoise(p*frequenzy);
	for(int curOctave = 0; curOctave < octaveCount; curOctave++)
	{
		signal = noise(p);
		signal = 2.0*abs(signal)-1.0;
		value += signal * curPersistence;
		p *= f_lacunarity;
		curPersistence *= f_persistence;
	}
	value += 0.5;
	return value;
}

float select(float low, float high, float select, vec2 bounds, float edgeFallOff)
{
	//return select;
	//float s = 0.5+0.5*select;
	float alpha  = SCurve3((select-bounds.x+edgeFallOff)/(2.0*edgeFallOff));
	float alpha2 = SCurve3((select-bounds.y+edgeFallOff)/(2.0*edgeFallOff));

	if(select < (bounds.x - edgeFallOff)) 		  return low;
	else if(select < (bounds.x + edgeFallOff)) 	  return LinearInterp(low, high, alpha );
	else if (select < (bounds.y - edgeFallOff)) return high;
	else if (select < (bounds.y + edgeFallOff)) return LinearInterp(high, low, alpha2);
	else return low;
}

float curve(float value, vec2 points[10], int pointsCount)
{
// Find the first element in the control point array that has an input value
  // larger than the output value from the source module.
  int indexPos;
  for (indexPos = 0; indexPos < pointsCount; indexPos++) 
    if (value < points[indexPos].x) break;

  // Find the four nearest control points so that we can perform cubic
  // interpolation.
  ivec4 index = ivec4(clampi(indexPos - 2, 0, pointsCount - 1),  // 0, x 
					  clampi(indexPos - 1, 0, pointsCount - 1),  // 1, y
					  clampi(indexPos    , 0, pointsCount - 1),  // 2, z
					  clampi(indexPos + 1, 0, pointsCount - 1)); // 3, w

  // If some control points are missing (which occurs if the value from the
  // source module is greater than the largest input value or less than the
  // smallest input value of the control point array), get the corresponding
  // output value of the nearest control point and exit now.
  if (index.y == index.z) {
    return points[index.y].y;
  }
  
  // Compute the alpha value used for cubic interpolation.
  vec2 inputv = vec2(points[index.y].x, points[index.z].x);
  float alpha = (value - inputv.x) / (inputv.y - inputv.x);

  // Now perform the cubic interpolation given the alpha value.
  return CubicInterp (vec4(points[index.x].y,
						   points[index.y].y,
						   points[index.z].y,
						   points[index.w].y), alpha);
}

float terrace(float value, float points[6], int pointsCount)
{
  // Find the first element in the control point array that has a value
  // larger than the output value from the source module.
  int indexPos;
  for (indexPos = 0; indexPos < pointsCount; indexPos++) {
    if (value < points[indexPos]) {
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
    return points[index.y];

  // Compute the alpha value used for linear interpolation.
  vec2 valuev = vec2(points[index.x], points[index.y]);
  float alpha = (value - valuev.x) / (valuev.y - valuev.x);

  // Squaring the alpha produces the terrace effect.
  alpha *= alpha;

  // Now perform the linear interpolation given the alpha value.
  return LinearInterp (valuev.x, valuev.y, alpha);
}


float blend(vec3 value)
{
  float alpha = (value.z + 1.0) / 2.0;
  return LinearInterp (value.x, value.y, alpha);
}

float exponent(float value, float fexponent)
{
  return (pow (abs ((value + 1.0) / 2.0), fexponent) * 2.0 - 1.0);
}

vec3 turbulence(vec3 value, float seed, float frequency, float power, int roughness)
{
  vec3 v0 = value + vec3(12414.0/65536.0, 65124.0/65536.0, 31337.0/65536.0) + seed;
  vec3 v1 = value + vec3(26519.0/65536.0, 18128.0/65536.0, 60493.0/65536.0) + seed;
  vec3 v2 = value + vec3(53820.0/65536.0, 11213.0/65536.0, 44845.0/65536.0) + seed;

  return value + vec3(sOctaveNoise(v0, frequency, roughness)*power,
		      sOctaveNoise(v1, frequency, roughness)*power,
		      sOctaveNoise(v2, frequency, roughness)*power);
}

vec3 packHeight(float height) 
{ 
	float b = floor(height * 256.0);
	float g = floor(height * 65536.0) - (b*256.0);
	float r = (floor(height * 16777216.0) - (b*65536.0)) - (g*256.0);
	return vec3(r, g, b)/ 256.0;
}

float unpackHeight(vec3 color)
{   
	return (color.r+color.g*256.0 + (color.b*256.0*256.0))/(256.0*256.0);
}

void main( void )
{
	//Default
	const float DEFAULT_LACUNARTITY = 2.0;

	f_lacunarity = DEFAULT_LACUNARTITY;
	f_persistence = 0.5;

	vec2 curveTemp[10];
	float terraceTemp[6];
	float n = 0;

	
	
	////////////////////////////////////////////////////////////////////////////
	// Module group: river positions
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	// Module subgroup: river positions (7 noise modules)
	//
	// This subgroup generates the river positions.
	//
	// -1.0 represents the lowest elevations and +1.0 represents the highest
	// elevations.
	//

	// 6: [Warped-rivers module]: This turbulence module warps the output value
  	//    from the combined-rivers module, which twists the rivers.  The high
  	//    roughness produces less-smooth rivers.
  	vec3 riverPositions_tu = turbulence(v_texCoord3D, 102, 9.25, 1.0 / 57.75, 6);
	
	// 1: [Large-river-basis module]: This ridged-multifractal-noise module
	//    creates the large, deep rivers.
	f_lacunarity = CONTINENT_LACUNARITY;
	float riverPositions_rm0 = sridged(riverPositions_tu+100, 18.75, 1);

	// 2: [Large-river-curve module]: This curve module applies a curve to the
	//    output value from the large-river-basis module so that the ridges
	//    become inverted.  This creates the rivers.  This curve also compresses
	//    the edge of the rivers, producing a sharp transition from the land to
	//    the river bottom.
	curveTemp[0] = vec2(-2.000,  2.000);
	curveTemp[1] = vec2(-1.000,  1.000);
	curveTemp[2] = vec2(-0.125,  0.875);
	curveTemp[3] = vec2( 0.000, -1.000);
	curveTemp[4] = vec2( 1.000, -1.500);
	curveTemp[5] = vec2( 2.000, -2.000);
	float riverPositions_cu0 = curve(riverPositions_rm0, curveTemp, 6);

	/// 3: [Small-river-basis module]: This ridged-multifractal-noise module
	//     creates the small, shallow rivers.
	float riverPositions_rm1 = sridged(riverPositions_tu+101, 43.25, 1);

	// 4: [Small-river-curve module]: This curve module applies a curve to the
	//    output value from the small-river-basis module so that the ridges
	//    become inverted.  This creates the rivers.  This curve also compresses
	//    the edge of the rivers, producing a sharp transition from the land to
	//    the river bottom.
	curveTemp[0] = vec2(-2.000,  2.0000);
	curveTemp[1] = vec2(-1.000,  1.5000);
	curveTemp[2] = vec2(-0.125,  1.4375);
	curveTemp[3] = vec2( 0.000,  0.5000);
	curveTemp[4] = vec2( 1.000,  0.2500);
	curveTemp[5] = vec2( 2.000,  0.0000);
	float riverPositions_cu1 = curve(riverPositions_rm1, curveTemp, 6);

	// 5: [Combined-rivers module]: This minimum-value module causes the small
	//    rivers to cut into the large rivers.  It does this by selecting the
	//    minimum output values from the large-river-curve module and the small-
	//    river-curve module.
	float riverPositions_mi = min(riverPositions_cu0, riverPositions_cu1);

	
	float continentsWithBadlands_ma = unpackHeight(texture2D(texture0, gl_TexCoord[0].xy).xyz*2.0-1.0);

	////////////////////////////////////////////////////////////////////////////
	// Module subgroup: continents with rivers (4 noise modules)
	//
	// This subgroup applies the river-positions group to the continents-with-
	// badlands subgroup.
	//
	// The output value from this module subgroup is measured in planetary
	// elevation units (-1.0 for the lowest underwater trenches and +1.0 for the
	// highest mountain peaks.)
	//

	// 1: [Scaled-rivers module]: This scale/bias module scales the output value
	//    from the river-positions group so that it is measured in planetary
	//    elevation units and is negative; this is required for step 2.
	float continentsWithRivers_sb = riverPositions_mi * RIVER_DEPTH / 2.0 - RIVER_DEPTH / 2.0;

	// 2: [Add-rivers-to-continents module]: This addition module adds the
	//    rivers to the continents-with-badlands subgroup.  Because the scaled-
	//    rivers module only outputs a negative value, the scaled-rivers module
	//    carves the rivers out of the terrain.
	float continentsWithRivers_ad = continentsWithBadlands_ma + continentsWithRivers_sb;

	// 3: [Blended-rivers-to-continents module]: This selector module outputs
	//    deep rivers near sea level and shallower rivers in higher terrain.  It
	//    does this by selecting the output value from the continents-with-
	//    badlands subgroup if the corresponding output value from the
	//    continents-with-badlands subgroup is far from sea level.  Otherwise,
	//    this selector module selects the output value from the add-rivers-to-
	//    continents module.
	float continentsWithRivers_se = select(continentsWithBadlands_ma, continentsWithRivers_ad,
										   continentsWithBadlands_ma, vec2(SEA_LEVEL, CONTINENT_HEIGHT_SCALE + SEA_LEVEL),
										   CONTINENT_HEIGHT_SCALE - SEA_LEVEL);


	if(n == 0.0)
	   	n = continentsWithRivers_se;
//*/
	vec3 temp = packHeight(n * 0.5 + 0.5);
	
	gl_FragColor = vec4(temp, n * 0.5+0.5);
		
}
