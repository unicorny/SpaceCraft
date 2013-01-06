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
uniform sampler2D texture0; // continental Definition
uniform sampler2D texture1; // terrain Type Definiton
uniform sampler2D texture2; // badlandSands

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

	
	//float baseContinentDef_pe0 = sOctaveNoise(v_texCoord3D, CONTINENT_FREQUENCY, 14);
//n = baseContinentDef_pe0;

	//turbulences must be calculated bevore
	

	  
	//float continentDef_se = BaseContinentDefinition_se(v_texCoord3D);
	float continentDef_se = unpackHeight(texture2D(texture0, gl_TexCoord[0].xy).xyz*2.0-1.0);
	//float continentDef_se_tu = BaseContinentDefinition_se(terrainTypeDef_tu);

	float terrainTypeDef_te = unpackHeight(texture2D(texture1, gl_TexCoord[0].xy).xyz*2.0-1.0);
	
	////////////////////////////////////////////////////////////////////////////
	// Module group: mountainous terrain
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	// Module subgroup: mountain base definition (9 noise modules)
	//
	// This subgroup generates the base-mountain elevations.  Other subgroups
	// will add the ridges and low areas to the base elevations.
	//
	// -1.0 represents low mountainous terrain and +1.0 represents high
	// mountainous terrain.
	//

	// 8: [Warped-mountains-and-valleys module]: This turbulence module warps
  	//    the output value from the coarse-turbulence module.  This turbulence
	//    has a higher frequency, but lower power, than the coarse-turbulence
	//    module, adding some fine detail to it.
	vec3 mountainBaseDef_tu1 = turbulence(v_texCoord3D, 33, 21221.0,  1.0 / 120157.0 * MOUNTAINS_TWIST, 6);

	// 7: [Coarse-turbulence module]: This turbulence module warps the output
  	//    value from the mountain-and-valleys module, adding some coarse detail
  	//    to it.
	vec3 mountainBaseDef_tu0 = turbulence(mountainBaseDef_tu1, 32, 1337.0, 1.0 / 6730.0 * MOUNTAINS_TWIST, 4);	

	// 1: [Mountain-ridge module]: This ridged-multifractal-noise module
	//    generates the mountain ridges.
	f_lacunarity = MOUNTAIN_LACUNARITY;
	float mountainBaseDef_rm0 = sridged(mountainBaseDef_tu0+30, 1723.0, 4);

	// 2: [Scaled-mountain-ridge module]: Next, a scale/bias module scales the
	//    output value from the mountain-ridge module so that its ridges are not
	//    too high.  The reason for this is that another subgroup adds actual
	//    mountainous terrain to these ridges.
	float mountainBaseDef_sb0 = mountainBaseDef_rm0 * 0.5 + 0.375;

	// 3: [River-valley module]: This ridged-multifractal-noise module generates
	//    the river valleys.  It has a much lower frequency than the mountain-
	//    ridge module so that more mountain ridges will appear outside of the
	//    valleys.  Note that this noise module generates ridged-multifractal
	//    noise using only one octave; this information will be important in the
	//    next step.
	float mountainBaseDef_rm1 = sridged(mountainBaseDef_tu0+31, 367.0, 1);

	// 4: [Scaled-river-valley module]: Next, a scale/bias module applies a
	//    scaling factor of -2.0 to the output value from the river-valley
	//    module.  This stretches the possible elevation values because one-
	//    octave ridged-multifractal noise has a lower range of output values
	//    than multiple-octave ridged-multifractal noise.  The negative scaling
	//    factor inverts the range of the output value, turning the ridges from
	//    the river-valley module into valleys.
	float mountainBaseDef_sb1 = mountainBaseDef_rm1 * -2.0 - 0.5;

	// 5: [Low-flat module]: This low constant value is used by step 6.
	float mountainBaseDef_co = -1.0;

	// 6: [Mountains-and-valleys module]: This blender module merges the
	//    scaled-mountain-ridge module and the scaled-river-valley module
	//    together.  It causes the low-lying areas of the terrain to become
	//    smooth, and causes the high-lying areas of the terrain to contain
	//    ridges.  To do this, it uses the scaled-river-valley module as the
	//    control module, causing the low-flat module to appear in the lower
	//    areas and causing the scaled-mountain-ridge module to appear in the
	//    higher areas.
	float mountainBaseDef_bl = blend(vec3(mountainBaseDef_co, mountainBaseDef_sb0, mountainBaseDef_sb1));

	////////////////////////////////////////////////////////////////////////////
	// Module subgroup: high mountainous terrain (5 noise modules)
	//
	// This subgroup generates the mountainous terrain that appears at high
	// elevations within the mountain ridges.
	//
	// -1.0 represents the lowest elevations and +1.0 represents the highest
	// elevations.
	//

	// 4: [Warped-high-mountains module]: This turbulence module warps the
  	//    output value from the high-mountains module, adding some detail to it.
  	vec3 mountainousHigh_tu = turbulence(v_texCoord3D, 42, 31511.0, 1.0 / 180371.0 * MOUNTAINS_TWIST, 4);
		
	// 1: [Mountain-basis-0 module]: This ridged-multifractal-noise module,
	//    along with the mountain-basis-1 module, generates the individual
	//    mountains.
	float mountainousHigh_rm0 = sridged(mountainousHigh_tu+40, 2371.0, 3);

	// 2: [Mountain-basis-1 module]: This ridged-multifractal-noise module,
	//    along with the mountain-basis-0 module, generates the individual
	//    mountains.
	float mountainousHigh_rm1 = sridged(mountainousHigh_tu+41, 2341.0, 3); 

	// 3: [High-mountains module]: Next, a maximum-value module causes more
	//    mountains to appear at the expense of valleys.  It does this by
	//    ensuring that only the maximum of the output values from the two
	//    ridged-multifractal-noise modules contribute to the output value of
	//    this subgroup.
	float mountainousHigh_ma = max(mountainousHigh_rm0, mountainousHigh_rm1);

	////////////////////////////////////////////////////////////////////////////
	// Module subgroup: low mountainous terrain (4 noise modules)
	//
	// This subgroup generates the mountainous terrain that appears at low
	// elevations within the river valleys.
	//
	// -1.0 represents the lowest elevations and +1.0 represents the highest
	// elevations.
	//

	// 1: [Lowland-basis-0 module]: This ridged-multifractal-noise module,
	//    along with the lowland-basis-1 module, produces the low mountainous
	//    terrain.
	float mountainousLow_rm0 = sridged(v_texCoord3D+50, 1381.0, 8);

	// 1: [Lowland-basis-1 module]: This ridged-multifractal-noise module,
	//    along with the lowland-basis-0 module, produces the low mountainous
	//    terrain.
	float mountainousLow_rm1 = sridged(v_texCoord3D+51, 1427.0, 8);

	// 3: [Low-mountainous-terrain module]: This multiplication module combines
	//    the output values from the two ridged-multifractal-noise modules.
	//    This causes the following to appear in the resulting terrain:
	//    - Cracks appear when two negative output values are multiplied
	//      together.
	//    - Flat areas appear when a positive and a negative output value are
	//      multiplied together.
	//    - Ridges appear when two positive output values are multiplied
	//      together.
	float mountainousLow_mu = mountainousLow_rm0 * mountainousLow_rm1;

	////////////////////////////////////////////////////////////////////////////
	// Module subgroup: mountainous terrain (7 noise modules)
	//
	// This subgroup generates the final mountainous terrain by combining the
	// high-mountainous-terrain subgroup with the low-mountainous-terrain
	// subgroup.
	//
	// -1.0 represents the lowest elevations and +1.0 represents the highest
	// elevations.
	//

	// 1: [Scaled-low-mountainous-terrain module]: First, this scale/bias module
	//    scales the output value from the low-mountainous-terrain subgroup to a
	//    very low value and biases it towards -1.0.  This results in the low
	//    mountainous areas becoming more-or-less flat with little variation.
	//    This will also result in the low mountainous areas appearing at the
	//    lowest elevations in this subgroup.
	float mountainousTerrain_sb0 = mountainousLow_mu * 0.03125 -0.96875; 

	// 2: [Scaled-high-mountainous-terrain module]: Next, this scale/bias module
	//    scales the output value from the high-mountainous-terrain subgroup to
	//    1/4 of its initial value and biases it so that its output value is
	//    usually positive.
	float mountainousTerrain_sb1 = mountainousHigh_ma * 0.25 + 0.25;

	// 3: [Added-high-mountainous-terrain module]: This addition module adds the
	//    output value from the scaled-high-mountainous-terrain module to the
	//    output value from the mountain-base-definition subgroup.  Mountains
	//    now appear all over the terrain.
	float mountainousTerrain_ad = mountainousTerrain_sb1 + mountainBaseDef_bl;

	// 4: [Combined-mountainous-terrain module]: Note that at this point, the
	//    entire terrain is covered in high mountainous terrain, even at the low
	//    elevations.  To make sure the mountains only appear at the higher
	//    elevations, this selector module causes low mountainous terrain to
	//    appear at the low elevations (within the valleys) and the high
	//    mountainous terrain to appear at the high elevations (within the
	//    ridges.)  To do this, this noise module selects the output value from
	//    the added-high-mountainous-terrain module if the output value from the
	//    mountain-base-definition subgroup is higher than a set amount.
	//    Otherwise, this noise module selects the output value from the scaled-
	//    low-mountainous-terrain module.
	float mountainousTerrain_se = select(mountainousTerrain_sb0, mountainousTerrain_ad,
										 mountainBaseDef_bl, vec2(-0.5, 999.5), 0.5);

	// 5: [Scaled-mountainous-terrain-module]: This scale/bias module slightly
	//    reduces the range of the output value from the combined-mountainous-
	//    terrain module, decreasing the heights of the mountain peaks.
	float mountainousTerrain_sb2 = mountainousTerrain_se * 0.8;

	// 6: [Glaciated-mountainous-terrain-module]: This exponential-curve module
	//    applies an exponential curve to the output value from the scaled-
	//    mountainous-terrain module.  This causes the slope of the mountains to
	//    smoothly increase towards higher elevations, as if a glacier grinded
	//    out those mountains.  This exponential-curve module expects the output
	//    value to range from -1.0 to +1.0.
	float mountainousTerrain_ex = exponent(mountainousTerrain_sb2, MOUNTAIN_GLACIATION);

	////////////////////////////////////////////////////////////////////////////
	// Module group: hilly terrain
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	// Module subgroup: hilly terrain (11 noise modules)
	//
	// This subgroup generates the hilly terrain.
	//
	// -1.0 represents the lowest elevations and +1.0 represents the highest
	// elevations.
	//

	// 10: [Warped-hilly-terrain module]: This turbulence module warps the
	//     output value from the coarse-turbulence module.  This turbulence has
  	//     a higher frequency, but lower power, than the coarse-turbulence
	//     module, adding some fine detail to it.
	vec3 hillyTerrain_tu1 = turbulence(v_texCoord3D, 63, 21617.0, 1.0 / 117529.0 * HILLS_TWIST, 6);

	// 9: [Coarse-turbulence module]: This turbulence module warps the output
	//    value from the increased-slope-hilly-terrain module, adding some
	//    coarse detail to it.
	vec3 hillyTerrain_tu0 = turbulence(hillyTerrain_tu1, 62, 1531.0, 1.0 / 16921.0 * HILLS_TWIST, 4); 
	
	// 1: [Hills module]: This billow-noise module generates the hills.
	f_lacunarity = HILLS_LACUNARITY;
	float hillyTerrain_bi = sbillow(hillyTerrain_tu0+60, 1663.0, 6);

	// 2: [Scaled-hills module]: Next, a scale/bias module scales the output
	//    value from the hills module so that its hilltops are not too high.
	//    The reason for this is that these hills are eventually added to the
	//    river valleys (see below.)
	float hillyTerrain_sb0 = hillyTerrain_bi * 0.5 + 0.5;

	// 3: [River-valley module]: This ridged-multifractal-noise module generates
	//    the river valleys.  It has a much lower frequency so that more hills
	//    will appear in between the valleys.  Note that this noise module
	//    generates ridged-multifractal noise using only one octave; this
	//    information will be important in the next step.
	float hillyTerrain_rm = sridged(hillyTerrain_tu0+61, 367.5, 1);

	// 4: [Scaled-river-valley module]: Next, a scale/bias module applies a
	//    scaling factor of -2.0 to the output value from the river-valley
	//    module.  This stretches the possible elevation values because one-
	//    octave ridged-multifractal noise has a lower range of output values
	//    than multiple-octave ridged-multifractal noise.  The negative scaling
	//    factor inverts the range of the output value, turning the ridges from
	//    the river-valley module into valleys.
	float hillyTerrain_sb1 = hillyTerrain_rm * -2.0 - 0.5;

	// 5: [Low-flat module]: This low constant value is used by step 6.
	float hillyTerrain_co = -1.0;

	// 6: [Mountains-and-valleys module]: This blender module merges the
	//    scaled-hills module and the scaled-river-valley module together.  It
	//    causes the low-lying areas of the terrain to become smooth, and causes
	//    the high-lying areas of the terrain to contain hills.  To do this, it
	//    uses the scaled-hills module as the control module, causing the low-
	//    flat module to appear in the lower areas and causing the scaled-river-
	//    valley module to appear in the higher areas.
	float hillyTerrain_bl = blend(vec3(hillyTerrain_co, hillyTerrain_sb1, hillyTerrain_sb0));

	// 7: [Scaled-hills-and-valleys module]: This scale/bias module slightly
	//    reduces the range of the output value from the hills-and-valleys
	//    module, decreasing the heights of the hilltops.
	float hillyTerrain_sb2 = hillyTerrain_bl * 0.75 - 0.25;

	// 8: [Increased-slope-hilly-terrain module]: To increase the hill slopes at
	//    higher elevations, this exponential-curve module applies an
	//    exponential curve to the output value the scaled-hills-and-valleys
	//    module.  This exponential-curve module expects the input value to
	//    range from -1.0 to 1.0.
	float hillyTerrain_ex = exponent(hillyTerrain_sb2, 1.375);

	////////////////////////////////////////////////////////////////////////////
	// Module group: plains terrain
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	// Module subgroup: plains terrain (7 noise modules)
	//
	// This subgroup generates the plains terrain.
	//
	// Because this subgroup will eventually be flattened considerably, the
	// types and combinations of noise modules that generate the plains are not
	// really that important; they only need to "look" interesting.
	//
	// -1.0 represents the lowest elevations and +1.0 represents the highest
	// elevations.
	//

	// 1: [Plains-basis-0 module]: This billow-noise module, along with the
	//    plains-basis-1 module, produces the plains.
	f_lacunarity = PLAINS_LACUNARITY;
	float plainsTerrain_bi0 = sbillow(v_texCoord3D+70, 1097.5, 8);

	// 2: [Positive-plains-basis-0 module]: This scale/bias module makes the
	//    output value from the plains-basis-0 module positive since this output
	//    value will be multiplied together with the positive-plains-basis-1
	//    module.
	float plainsTerrain_sb0 = plainsTerrain_bi0 * 0.5 + 0.5;

	// 3: [Plains-basis-1 module]: This billow-noise module, along with the
	//    plains-basis-2 module, produces the plains.
	float plainsTerrain_bi1 = sbillow(v_texCoord3D+71, 1319.5, 8);

	// 4: [Positive-plains-basis-1 module]: This scale/bias module makes the
	//    output value from the plains-basis-1 module positive since this output
	//    value will be multiplied together with the positive-plains-basis-0
	//    module.
	float plainsTerrain_sb1 = plainsTerrain_bi1 * 0.5 + 0.5;

	// 5: [Combined-plains-basis module]: This multiplication module combines
	//    the two plains basis modules together.
	float plainsTerrain_mu = plainsTerrain_sb0 * plainsTerrain_sb1;

	// 6: [Rescaled-plains-basis module]: This scale/bias module maps the output
	//    value that ranges from 0.0 to 1.0 back to a value that ranges from
	//    -1.0 to +1.0.
	float plainsTerrain_sb2 = plainsTerrain_mu * 2.0 -1.0;

	////////////////////////////////////////////////////////////////////////////
	// Module group: scaled mountainous terrain
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	// Module subgroup: scaled mountainous terrain (6 noise modules)
	//
	// This subgroup scales the output value from the mountainous-terrain group
	// so that it can be added to the elevation defined by the continent-
	// definition group.
	//
	// This subgroup scales the output value such that it is almost always
	// positive.  This is done so that a negative elevation does not get applied
	// to the continent-definition group, preventing parts of that group from
	// having negative terrain features "stamped" into it.
	//
	// The output value from this module subgroup is measured in planetary
	// elevation units (-1.0 for the lowest underwater trenches and +1.0 for the
	// highest mountain peaks.)
	//

	// 1: [Base-scaled-mountainous-terrain module]: This scale/bias module
	//    scales the output value from the mountainous-terrain group so that the
	//    output value is measured in planetary elevation units.
	float scaledMountainousTerrain_sb0 = mountainousTerrain_ex*0.125 + 0.125;

	// 2: [Base-peak-modulation module]: At this stage, most mountain peaks have
	//    roughly the same elevation.  This Perlin-noise module generates some
	//    random values that will be used by subsequent noise modules to
	//    randomly change the elevations of the mountain peaks.
	f_lacunarity = MOUNTAIN_LACUNARITY;
	float scaledMountainousTerrain_pe = sOctaveNoise(v_texCoord3D+110, 14.5, 6);

	// 3: [Peak-modulation module]: This exponential-curve module applies an
	//    exponential curve to the output value from the base-peak-modulation
	//    module.  This produces a small number of high values and a much larger
	//    number of low values.  This means there will be a few peaks with much
	//    higher elevations than the majority of the peaks, making the terrain
	//    features more varied.
	float scaledMountainousTerrain_ex = exponent(scaledMountainousTerrain_pe, 1.25);

	// 4: [Scaled-peak-modulation module]: This scale/bias module modifies the
	//    range of the output value from the peak-modulation module so that it
	//    can be used as the modulator for the peak-height-multiplier module.
	//    It is important that this output value is not much lower than 1.0.
	float scaledMountainousTerrain_sb1 = scaledMountainousTerrain_ex * 0.25+1.0;

	// 5: [Peak-height-multiplier module]: This multiplier module modulates the
	//    heights of the mountain peaks from the base-scaled-mountainous-terrain
	//    module using the output value from the scaled-peak-modulation module.
	float scaledMountainousTerrain_mu = scaledMountainousTerrain_sb0 * scaledMountainousTerrain_sb1;

	////////////////////////////////////////////////////////////////////////////
	// Module group: scaled hilly terrain
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	// Module subgroup: scaled hilly terrain (6 noise modules)
	//
	// This subgroup scales the output value from the hilly-terrain group so
	// that it can be added to the elevation defined by the continent-
	// definition group.  The scaling amount applied to the hills is one half of
	// the scaling amount applied to the scaled-mountainous-terrain group.
	//
	// This subgroup scales the output value such that it is almost always
	// positive.  This is done so that negative elevations are not applied to
	// the continent-definition group, preventing parts of the continent-
	// definition group from having negative terrain features "stamped" into it.
	//
	// The output value from this module subgroup is measured in planetary
	// elevation units (-1.0 for the lowest underwater trenches and +1.0 for the
	// highest mountain peaks.)
	//

	// 1: [Base-scaled-hilly-terrain module]: This scale/bias module scales the
	//    output value from the hilly-terrain group so that this output value is
	//    measured in planetary elevation units 
	float scaledHillyTerrain_sb0 = hillyTerrain_ex * 0.0625 + 0.0625;

	// 2: [Base-hilltop-modulation module]: At this stage, most hilltops have
	//    roughly the same elevation.  This Perlin-noise module generates some
	//    random values that will be used by subsequent noise modules to
	//    randomly change the elevations of the hilltops.
	f_lacunarity = HILLS_LACUNARITY;
	float scaledHillyTerrain_pe = sOctaveNoise(v_texCoord3D+120, 13.5, 6);

	// 3: [Hilltop-modulation module]: This exponential-curve module applies an
	//    exponential curve to the output value from the base-hilltop-modulation
	//    module.  This produces a small number of high values and a much larger
	//    number of low values.  This means there will be a few hilltops with
	//    much higher elevations than the majority of the hilltops, making the
	//    terrain features more varied.
	float scaledHillyTerrain_ex = exponent(scaledHillyTerrain_pe, 1.25);

	// 4: [Scaled-hilltop-modulation module]: This scale/bias module modifies
	//    the range of the output value from the hilltop-modulation module so
	//    that it can be used as the modulator for the hilltop-height-multiplier
	//    module.  It is important that this output value is not much lower than
	//    1.0.
	float scaledHillyTerrain_sb1 = scaledHillyTerrain_ex * 0.5 + 1.5;

	// 5: [Hilltop-height-multiplier module]: This multiplier module modulates
	//    the heights of the hilltops from the base-scaled-hilly-terrain module
	//    using the output value from the scaled-hilltop-modulation module.
	float scaledHillyTerrain_mu = scaledHillyTerrain_sb0 * scaledHillyTerrain_sb1;
	
	////////////////////////////////////////////////////////////////////////////
	// Module group: scaled plains terrain
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	// Module subgroup: scaled plains terrain (2 noise modules)
	//
	// This subgroup scales the output value from the plains-terrain group so
	// that it can be added to the elevations defined by the continent-
	// definition group.
	//
	// This subgroup scales the output value such that it is almost always
	// positive.  This is done so that negative elevations are not applied to
	// the continent-definition group, preventing parts of the continent-
	// definition group from having negative terrain features "stamped" into it.
	//
	// The output value from this module subgroup is measured in planetary
	// elevation units (-1.0 for the lowest underwater trenches and +1.0 for the
	// highest mountain peaks.)
	//

	// 1: [Scaled-plains-terrain module]: This scale/bias module greatly
	//    flattens the output value from the plains terrain.  This output value
	//    is measured in planetary elevation units 
	float scaledPlainsTerrain_sb = plainsTerrain_sb2 * 0.00390625 + 0.0078125;

	
	float badlandsTerrain_ma = unpackHeight(texture2D(texture2, gl_TexCoord[0].xy).xyz*2.0-1.0);
	////////////////////////////////////////////////////////////////////////////
	// Module group: scaled badlands terrain
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	// Module subgroup: scaled badlands terrain (2 noise modules)
	//
	// This subgroup scales the output value from the badlands-terrain group so
	// that it can be added to the elevations defined by the continent-
	// definition group.
	//
	// This subgroup scales the output value such that it is almost always
	// positive.  This is done so that negative elevations are not applied to
	// the continent-definition group, preventing parts of the continent-
	// definition group from having negative terrain features "stamped" into it.
	//
	// The output value from this module subgroup is measured in planetary
	// elevation units (-1.0 for the lowest underwater trenches and +1.0 for the
	// highest mountain peaks.)
	//

	// 1: [Scaled-badlands-terrain module]: This scale/bias module scales the
	//    output value from the badlands-terrain group so that it is measured
	//    in planetary elevation units 
	float scaledBadlandsTerrain_sb = badlandsTerrain_ma * 0.00625 + 0.00625;

	////////////////////////////////////////////////////////////////////////////
	// Module group: final planet
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	// Module subgroup: continental shelf (6 noise modules)
	//
	// This module subgroup creates the continental shelves.
	//
	// The output value from this module subgroup are measured in planetary
	// elevation units (-1.0 for the lowest underwater trenches and +1.0 for the
	// highest mountain peaks.)
	//

	// 1: [Shelf-creator module]: This terracing module applies a terracing
	//    curve to the continent-definition group at the specified shelf level.
	//    This terrace becomes the continental shelf.  Note that this terracing
	//    module also places another terrace below the continental shelf near
	//    -1.0.  The bottom of this terrace is defined as the bottom of the
	//    ocean; subsequent noise modules will later add oceanic trenches to the
	//    bottom of the ocean.
	terraceTemp[0] = -1.0;
	terraceTemp[1] = -0.75;
	terraceTemp[2] = SHELF_LEVEL;
	terraceTemp[3] = 1.0;
	float continentalShelf_te = terrace(continentDef_se, terraceTemp, 4);

	// 2: [Oceanic-trench-basis module]: This ridged-multifractal-noise module
	//    generates some coherent noise that will be used to generate the
	//    oceanic trenches.  The ridges represent the bottom of the trenches.
	f_lacunarity = CONTINENT_LACUNARITY;
	float continentalShelf_rm = sridged(v_texCoord3D+130, CONTINENT_FREQUENCY * 4.375, 16);

	// 3: [Oceanic-trench module]: This scale/bias module inverts the ridges
	//    from the oceanic-trench-basis-module so that the ridges become
	//    trenches.  This noise module also reduces the depth of the trenches so
	//    that their depths are measured in planetary elevation units.
	float continentalShelf_sb = continentalShelf_rm * (-0.125) - 0.125;

	// 4: [Clamped-sea-bottom module]: This clamping module clamps the output
	//    value from the shelf-creator module so that its possible range is
	//    from the bottom of the ocean to sea level.  This is done because this
	//    subgroup is only concerned about the oceans.
	float continentalShelf_cl = clampf(continentalShelf_te, -0.75, SEA_LEVEL);

	// 5: [Shelf-and-trenches module]: This addition module adds the oceanic
	//    trenches to the clamped-sea-bottom module.
	float continentalShelf_ad = continentalShelf_sb + continentalShelf_cl;

	////////////////////////////////////////////////////////////////////////////
	// Module group: base continent elevations (3 noise modules)
	//
	// This subgroup generates the base elevations for the continents, before
	// terrain features are added.
	//
	// The output value from this module subgroup is measured in planetary
	// elevation units (-1.0 for the lowest underwater trenches and +1.0 for the
	// highest mountain peaks.)
	//

	// 1: [Base-scaled-continent-elevations module]: This scale/bias module
	//    scales the output value from the continent-definition group so that it
	//    is measured in planetary elevation units 
	float baseContinentElev_sb = continentDef_se * CONTINENT_HEIGHT_SCALE;

	// 2: [Base-continent-with-oceans module]: This selector module applies the
	//    elevations of the continental shelves to the base elevations of the
	//    continent.  It does this by selecting the output value from the
	//    continental-shelf subgroup if the corresponding output value from the
	//    continent-definition group is below the shelf level.  Otherwise, it
	//    selects the output value from the base-scaled-continent-elevations
	//    module.
	float baseContinentElev_se = select(baseContinentElev_sb, continentalShelf_ad, continentDef_se, 
										vec2(SHELF_LEVEL - 1000.0, SHELF_LEVEL), 0.03125);

	////////////////////////////////////////////////////////////////////////////
	// Module subgroup: continents with plains (2 noise modules)
	//
	// This subgroup applies the scaled-plains-terrain group to the base-
	// continent-elevation subgroup.
	//
	// The output value from this module subgroup is measured in planetary
	// elevation units (-1.0 for the lowest underwater trenches and +1.0 for the
	// highest mountain peaks.)
	//

	// 1: [Continents-with-plains module]:  This addition module adds the
	//    scaled-plains-terrain group to the base-continent-elevation subgroup.
	float continentsWithPlains_ad = baseContinentElev_se + scaledPlainsTerrain_sb;
	
	////////////////////////////////////////////////////////////////////////////
	// Module subgroup: continents with hills (3 noise modules)
	//
	// This subgroup applies the scaled-hilly-terrain group to the continents-
	// with-plains subgroup.
	//
	// The output value from this module subgroup is measured in planetary
	// elevation units (-1.0 for the lowest underwater trenches and +1.0 for the
	// highest mountain peaks.)
	//

	// 1: [Continents-with-hills module]:  This addition module adds the scaled-
	//    hilly-terrain group to the base-continent-elevation subgroup.
	float continentsWithHills_ad = baseContinentElev_se + scaledHillyTerrain_mu;

	// 2: [Select-high-elevations module]: This selector module ensures that
	//    the hills only appear at higher elevations.  It does this by selecting
	//    the output value from the continent-with-hills module if the
	//    corresponding output value from the terrain-type-defintion group is
	//    above a certain value. Otherwise, it selects the output value from the
	//    continents-with-plains subgroup.
	float continentsWithHills_se = select(continentsWithPlains_ad, continentsWithHills_ad,
										  terrainTypeDef_te, vec2(1.0-HILLS_AMOUNT, 1001.0 - HILLS_AMOUNT), 0.25);
	////////////////////////////////////////////////////////////////////////////
	// Module subgroup: continents with mountains (5 noise modules)
	//
	// This subgroup applies the scaled-mountainous-terrain group to the
	// continents-with-hills subgroup.
	//
	// The output value from this module subgroup is measured in planetary
	// elevation units (-1.0 for the lowest underwater trenches and +1.0 for the
	// highest mountain peaks.)
	//

	// 1: [Continents-and-mountains module]:  This addition module adds the
	//    scaled-mountainous-terrain group to the base-continent-elevation
	//    subgroup.
	float continentsWithMountains_ad0 = baseContinentElev_se + scaledMountainousTerrain_mu;

	// 2: [Increase-mountain-heights module]:  This curve module applies a curve
	//    to the output value from the continent-definition group.  This
	//    modified output value is used by a subsequent noise module to add
	//    additional height to the mountains based on the current continent
	//    elevation.  The higher the continent elevation, the higher the
	//    mountains.
	curveTemp[0] =  vec2(                  -1.0, -0.0625);
	curveTemp[1] =  vec2(                   0.0,  0.0000);
	curveTemp[2] =  vec2(1.0 - MOUNTAINS_AMOUNT,  0.0625);
	curveTemp[3] =  vec2(                   1.0,  0.2500);
	float continentsWithMountains_cu = curve(continentDef_se, curveTemp, 4);

	// 3: [Add-increased-mountain-heights module]: This addition module adds
	//    the increased-mountain-heights module to the continents-and-
	//    mountains module.  The highest continent elevations now have the
	//    highest mountains.
	float continentsWithMountains_ad1 = continentsWithMountains_ad0 + continentsWithMountains_cu;

	// 4: [Select-high-elevations module]: This selector module ensures that
	//    mountains only appear at higher elevations.  It does this by selecting
	//    the output value from the continent-with-mountains module if the
	//    corresponding output value from the terrain-type-defintion group is
	//    above a certain value.  Otherwise, it selects the output value from
	//    the continents-with-hills subgroup.  Note that the continents-with-
	//    hills subgroup also contains the plains terrain.
	float continentsWithMountains_se = select(continentsWithHills_se, continentsWithMountains_ad1,
											  terrainTypeDef_te, vec2(1.0 - MOUNTAINS_AMOUNT, 1001.0 - MOUNTAINS_AMOUNT),
											  0.25);
	////////////////////////////////////////////////////////////////////////////
	// Module subgroup: continents with badlands (5 noise modules)
	//
	// This subgroup applies the scaled-badlands-terrain group to the
	// continents-with-mountains subgroup.
	//
	// The output value from this module subgroup is measured in planetary
	// elevation units (-1.0 for the lowest underwater trenches and +1.0 for the
	// highest mountain peaks.)
	//

	// 1: [Badlands-positions module]: This Perlin-noise module generates some
	//    random noise, which is used by subsequent noise modules to specify the
	//    locations of the badlands.
	f_lacunarity = CONTINENT_LACUNARITY;
	float continentsWithBadlands_pe = sOctaveNoise(v_texCoord3D+140, 16.5, 2);

	// 2: [Continents-and-badlands module]:  This addition module adds the
	//    scaled-badlands-terrain group to the base-continent-elevation
	//    subgroup.
	float continentsWithBadlands_ad = baseContinentElev_se + scaledBadlandsTerrain_sb;

	// 3: [Select-badlands-positions module]: This selector module places
	//    badlands at random spots on the continents based on the Perlin noise
	//    generated by the badlands-positions module.  To do this, it selects
	//    the output value from the continents-and-badlands module if the
	//    corresponding output value from the badlands-position module is
	//    greater than a specified value.  Otherwise, this selector module
	//    selects the output value from the continents-with-mountains subgroup.
	//    There is also a wide transition between these two noise modules so
	//    that the badlands can blend into the rest of the terrain on the
	//    continents.
	float continentsWithBadlands_se = select(continentsWithMountains_se, continentsWithBadlands_ad,
											 continentsWithBadlands_pe, vec2(1.0 - BADLANDS_AMOUNT,
											 1001.0 - BADLANDS_AMOUNT), 0.25);

	// 4: [Apply-badlands module]: This maximum-value module causes the badlands
	//    to "poke out" from the rest of the terrain.  It does this by ensuring
	//    that only the maximum of the output values from the continents-with-
	//    mountains subgroup and the select-badlands-positions modules
	//    contribute to the output value of this subgroup.  One side effect of
	//    this process is that the badlands will not appear in mountainous
	//    terrain.
	float continentsWithBadlands_ma = max(continentsWithMountains_se, continentsWithBadlands_se);

	

	if(n == 0.0)
	   	n = continentsWithBadlands_ma;
//*/
	vec3 temp = packHeight(n * 0.5 + 0.5);
	
	gl_FragColor = vec4(temp, n * 0.5+0.5);
		
}
