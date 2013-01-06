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

varying vec3 v_texCoord3D;

uniform float CONTINENT_FREQUENCY;
uniform float CONTINENT_LACUNARITY;
uniform float SEA_LEVEL;

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
vec3 fade(vec3 t) {
	return t*t*t*(t*(t*6.0-15.0)+10.0);
}
  
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


//#extension GL_EXT_gpu_shader4 : enable

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


vec3 turbulence(vec3 value, float seed, float frequency, float power, int roughness)
{
  vec3 v0 = value + vec3(12414.0/65536.0, 65124.0/65536.0, 31337.0/65536.0) + seed;
  vec3 v1 = value + vec3(26519.0/65536.0, 18128.0/65536.0, 60493.0/65536.0) + seed;
  vec3 v2 = value + vec3(53820.0/65536.0, 11213.0/65536.0, 44845.0/65536.0) + seed;

  return value + vec3(sOctaveNoise(v0, frequency, roughness)*power,
		      sOctaveNoise(v1, frequency, roughness)*power,
		      sOctaveNoise(v2, frequency, roughness)*power);
}

float BaseContinentDefinition(vec3 value)
{
	vec2 curveTemp[10];
	
	////////////////////////////////////////////////////////////////////////////
	// Module group: continent definition
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	// Module subgroup: base continent definition (7 noise modules)
	//
	// This subgroup roughly defines the positions and base elevations of the
	// planet's continents.
	//
	// The "base elevation" is the elevation of the terrain before any terrain
	// features (mountains, hills, etc.) are placed on that terrain.
	//
	// -1.0 represents the lowest elevations and +1.0 represents the highest
	// elevations.
	//
	// 1: [Continent module]: This Perlin-noise module generates the continents.
	//    This noise module has a high number of octaves so that detail is
	//    visible at high zoom levels.
	f_lacunarity = CONTINENT_LACUNARITY;
	float baseContinentDef_pe0 = sOctaveNoise(value, CONTINENT_FREQUENCY, 14);


	// 2: [Continent-with-ranges module]: Next, a curve module modifies the
	//    output value from the continent module so that very high values appear
	//    near sea level.  This defines the positions of the mountain ranges.	
	curveTemp[0] = vec2(-2.0000 + SEA_LEVEL,-1.625 + SEA_LEVEL);
	curveTemp[1] = vec2(-1.0000 + SEA_LEVEL,-1.375 + SEA_LEVEL);
	curveTemp[2] = vec2( 0.0000 + SEA_LEVEL,-0.375 + SEA_LEVEL);
	curveTemp[3] = vec2( 0.0625 + SEA_LEVEL, 0.125 + SEA_LEVEL);
	curveTemp[4] = vec2( 0.1250 + SEA_LEVEL, 0.250 + SEA_LEVEL);
	curveTemp[5] = vec2( 0.2500 + SEA_LEVEL, 1.000 + SEA_LEVEL);
	curveTemp[6] = vec2( 0.5000 + SEA_LEVEL, 0.250 + SEA_LEVEL);
	curveTemp[7] = vec2( 0.7500 + SEA_LEVEL, 0.250 + SEA_LEVEL);
	curveTemp[8] = vec2( 1.0000 + SEA_LEVEL, 0.500 + SEA_LEVEL);
	curveTemp[9] = vec2( 2.0000 + SEA_LEVEL, 0.500 + SEA_LEVEL);
	float baseContinentDef_cu = curve(baseContinentDef_pe0, curveTemp, 10);

	// 3: [Carver module]: This higher-frequency Perlin-noise module will be
	//    used by subsequent noise modules to carve out chunks from the mountain
	//    ranges within the continent-with-ranges module so that the mountain
	//    ranges will not be complely impassible.
	float baseContinentDef_pe1 = sOctaveNoise(value+1, CONTINENT_FREQUENCY * 4.34375, 11);

	// 4: [Scaled-carver module]: This scale/bias module scales the output
	//    value from the carver module such that it is usually near 1.0.  This
	//    is required for step 5.
	float baseContinentDef_sb = baseContinentDef_pe1 * 0.375 + 0.625;

	// 5: [Carved-continent module]: This minimum-value module carves out chunks
	//    from the continent-with-ranges module.  It does this by ensuring that
	//    only the minimum of the output values from the scaled-carver module
	//    and the continent-with-ranges module contributes to the output value
	//    of this subgroup.  Most of the time, the minimum-value module will
	//    select the output value from the continents-with-ranges module since
	//    the output value from the scaled-carver module is usually near 1.0.
	//    Occasionally, the output value from the scaled-carver module will be
	//    less than the output value from the continent-with-ranges module, so
	//    in this case, the output value from the scaled-carver module is
	//    selected.	
	float baseContinentDef_mi = min(baseContinentDef_sb, baseContinentDef_cu);

	// 6: [Clamped-continent module]: Finally, a clamp module modifies the
	//    carved-continent module to ensure that the output value of this
	//    subgroup is between -1.0 and 1.0.
	float baseContinentDef_cl = clampf(baseContinentDef_mi, -1.0, 1.0);

	return baseContinentDef_cl;
}

float BaseContinentDefinition_se(vec3 value)
{
	////////////////////////////////////////////////////////////////////////////
	// Module subgroup: continent definition (5 noise modules)
	//
	// This subgroup warps the output value from the the base-continent-
	// definition subgroup, producing more realistic terrain.
	//
	// Warping the base continent definition produces lumpier terrain with
	// cliffs and rifts.
	//
	// -1.0 represents the lowest elevations and +1.0 represents the highest
	// elevations.
	//
	
	//return BaseContinentDefinition(value);
	
	// 3: [Warped-base-continent-definition module]: This turbulence module
	//    warps the output value from the intermediate-turbulence module.  This
  	//    turbulence has a higher frequency, but lower power, than the
  	//    intermediate-turbulence module, adding some fine detail to it.
	vec3 continentDef_tu2 = turbulence(value, 12, CONTINENT_FREQUENCY * 95.25, CONTINENT_FREQUENCY / 1019.75, 11);

	// 2: [Intermediate-turbulence module]: This turbulence module warps the
  	//    output value from the coarse-turbulence module.  This turbulence has
  	//    a higher frequency, but lower power, than the coarse-turbulence
  	//    module, adding some intermediate detail to it.
	vec3 continentDef_tu1 = turbulence(continentDef_tu2, 11, CONTINENT_FREQUENCY * 47.25, CONTINENT_FREQUENCY / 433.75, 12);

	// 1: [Coarse-turbulence module]: This turbulence module warps the output
  	//    value from the base-continent-definition subgroup, adding some coarse
  	//    detail to it.
	vec3 continentDef_tu0 = turbulence(continentDef_tu1, 10, CONTINENT_FREQUENCY * 15.25, CONTINENT_FREQUENCY / 113.75, 13);
	
	
	
	float baseContinentDef_cl = BaseContinentDefinition(value);
	float baseContinentDef_cl_tu0 = BaseContinentDefinition(continentDef_tu0);

	// Module subgroup: continent definition (5 noise modules) (turbulencec are calculated bevore)
	// 4: [Select-turbulence module]: At this stage, the turbulence is applied
  	//    to the entire base-continent-definition subgroup, producing some very
  	//    rugged, unrealistic coastlines.  This selector module selects the
  	//    output values from the (unwarped) base-continent-definition subgroup
  	//    and the warped-base-continent-definition module, based on the output
  	//    value from the (unwarped) base-continent-definition subgroup.  The
  	//    selection boundary is near sea level and has a relatively smooth
  	//    transition.  In effect, only the higher areas of the base-continent-
  	//    definition subgroup become warped; the underwater and coastal areas
  	//    remain unaffected.
	float continentDef_se = select(baseContinentDef_cl, baseContinentDef_cl_tu0,
				       baseContinentDef_cl, vec2(SEA_LEVEL-0.0375, SEA_LEVEL+1000.0375), 0.0625);
	return continentDef_se;
	//*/
}

vec3 packHeight(float height) 
{ 
	float b = floor(height * 256.0);
	float g = floor(height * 65536.0) - (b*256.0);
	float r = (floor(height * 16777216.0) - (b*65536.0)) - (g*256.0);
	return vec3(r, g, b)/ 256.0;
}

void main( void )
{
	//Default
	const float DEFAULT_LACUNARTITY = 2.0;

	f_lacunarity = DEFAULT_LACUNARTITY;
	f_persistence = 0.5;

	float n = BaseContinentDefinition_se(v_texCoord3D);
//*/
	vec3 temp = packHeight(n * 0.5 + 0.5);

	gl_FragColor = vec4(temp, n * 0.5+0.5);
	
}
