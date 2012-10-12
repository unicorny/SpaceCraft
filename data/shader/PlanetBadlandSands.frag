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
uniform float BADLANDS_LACUNARITY;
uniform float BADLANDS_TWIST;

float f_lacunarity;
float f_persistence;

/*
 * To create offsets of one texel and one half texel in the
 * texture lookup, we need to know the texture image size.
 */
#define ONE 0.00390625
#define ONEHALF 0.001953125
// The numbers above are 1/256 and 0.5/256, change accordingly
// if you change the code to use another texture size.


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


float svoronoi(vec3 p, float frequenzy)
{	
	p *= frequenzy;
	ivec3 i = ivec3(floor(p));
	vec3 f = p - vec3(i);

	float dist1 = 9999999.0;
	float dist2 = 9999999.0;
	vec2 distv = vec2(9999999.0);
	vec3 cell;
	
	//vec2 cell;

	ivec3 index = ivec3(-1);
	for (index.z = -1; index.z <= 1; index.z++)
	{
		for (index.y = -1; index.y <= 1; index.y++)
		{
			for (index.x = -1; index.x <= 1; index.x++)
			{
				float noise = 0.5 + 0.5 * noise(i+index);
				cell = vec3(noise);
				cell += vec3(index)-f;
				float dist = dot(cell, cell);
				
				distv.x = (dist < distv.x) ? dist : distv.x;
			
				if (dist < distv.x)
				{
					distv.y = distv.x;
					distv.x = dist;
				}
				else if (dist < distv.y)
				{
					distv.y = dist;
				}//*/
			}
		}
	}	
	return vec2(sqrt(distv)).x*2.0-1.0;
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

void main( void )
{
	//Default
	const float DEFAULT_LACUNARTITY = 2.0;

	f_lacunarity = DEFAULT_LACUNARTITY;
	f_persistence = 0.5;
	
	vec2 curveTemp[10];
	float terraceTemp[6];

	////////////////////////////////////////////////////////////////////////////
	// Module group: badlands terrain
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	// Module subgroup: badlands sand (6 noise modules)
	//
	// This subgroup generates the sandy terrain for the badlands.
	//
	// -1.0 represents the lowest elevations and +1.0 represents the highest
	// elevations.
	//

	// 1: [Sand-dunes module]: This ridged-multifractal-noise module generates
	//    sand dunes.  This ridged-multifractal noise is generated with a single
	//    octave, which makes very smooth dunes.
	f_lacunarity = BADLANDS_LACUNARITY;
	float badlandsSand_rm = sridged(v_texCoord3D+80, 6163.5, 1);

	// 2: [Scaled-sand-dunes module]: This scale/bias module shrinks the dune
	//    heights by a small amount.  This is necessary so that the subsequent
	//    noise modules in this subgroup can add some detail to the dunes.
	float badlandsSand_sb0 = badlandsSand_rm * 0.875;

	// 3: [Dune-detail module]: This noise module uses Voronoi polygons to
	//    generate the detail to add to the dunes.  By enabling the distance
	//    algorithm, small polygonal pits are generated; the edges of the pits
	//    are joined to the edges of nearby pits.
	float badlandsSand_vo = svoronoi(v_texCoord3D, 16183.25);//16183.25);
	//badlandsSand_vo = voronoi(v_texCoord3D, 81, 16183.25, 1.0, false);

	// 4: [Scaled-dune-detail module]: This scale/bias module shrinks the dune
	//    details by a large amount.  This is necessary so that the subsequent
	//    noise modules in this subgroup can add this detail to the sand-dunes
	//    module.
	float badlandsSand_sb1 = badlandsSand_vo * 0.25+0.25;

	// 5: [Dunes-with-detail module]: This addition module combines the scaled-
	//    sand-dunes module with the scaled-dune-detail module.
	float badlandsSand_ad = badlandsSand_sb0 + badlandsSand_sb1;

	////////////////////////////////////////////////////////////////////////////
	// Module subgroup: badlands cliffs (7 noise modules)
	//
	// This subgroup generates the cliffs for the badlands.
	//
	// -1.0 represents the lowest elevations and +1.0 represents the highest
	// elevations.
	//

	// 6: [Warped-cliffs module]: This turbulence module warps the output value
  	//    from the coarse-turbulence module.  This turbulence has a higher
  	//    frequency, but lower power, than the coarse-turbulence module, adding
  	//    some fine detail to it.
	vec3 badlandsCliffs_tu1 = turbulence(v_texCoord3D, 92, 36107.0, 1.0 / 211543.0 * BADLANDS_TWIST, 3);

	// 5: [Coarse-turbulence module]: This turbulence module warps the output
  	//    value from the terraced-cliffs module, adding some coarse detail to
  	//    it.
	vec3 badlandsCliffs_tu0 = turbulence(badlandsCliffs_tu1, 91, 16111.0, 1.0 / 141539.0 * BADLANDS_TWIST, 3);
	  
	// 1: [Cliff-basis module]: This Perlin-noise module generates some coherent
	//    noise that will be used to generate the cliffs.
	float badlandsCliffs_pe = sOctaveNoise(badlandsCliffs_tu0+90, CONTINENT_FREQUENCY * 839.0, 6);

	// 2: [Cliff-shaping module]: Next, this curve module applies a curve to the
	//    output value from the cliff-basis module.  This curve is initially
	//    very shallow, but then its slope increases sharply.  At the highest
	//    elevations, the curve becomes very flat again.  This produces the
	//    stereotypical Utah-style desert cliffs.
	curveTemp[0] = vec2(-2.0000, -2.0000);
	curveTemp[1] = vec2(-1.0000, -1.2500);
	curveTemp[2] = vec2(-0.0000, -0.7500);
	curveTemp[3] = vec2( 0.5000, -0.2500);
	curveTemp[4] = vec2( 0.6250,  0.8750);
	curveTemp[5] = vec2( 0.7500,  1.0000);
	curveTemp[6] = vec2( 2.0000,  1.2500);
	float badlandsCliffs_cu = curve(badlandsCliffs_pe, curveTemp, 7);

	// 3: [Clamped-cliffs module]: This clamping module makes the tops of the
	//    cliffs very flat by clamping the output value from the cliff-shaping
	//    module so that the tops of the cliffs are very flat.
	float badlandsCliffs_cl = clamp(badlandsCliffs_cu, -999.125, 0.875);

	// 4: [Terraced-cliffs module]: Next, this terracing module applies some
	//    terraces to the clamped-cliffs module in the lower elevations before
	//    the sharp cliff transition.
	terraceTemp[0] = -1.0000;
	terraceTemp[1] = -0.8750;
	terraceTemp[2] = -0.7500;
	terraceTemp[3] = -0.5000;
	terraceTemp[4] =  0.0000;
	terraceTemp[5] =  1.0000;
	float badlandsCliffs_te = terrace(badlandsCliffs_cl, terraceTemp, 6);
	//n = badlandsCliffs_te;

	////////////////////////////////////////////////////////////////////////////
	// Module subgroup: badlands terrain (3 noise modules)
	//
	// Generates the final badlands terrain.
	//
	// Using a scale/bias module, the badlands sand is flattened considerably,
	// then the sand elevations are lowered to around -1.0.  The maximum value
	// from the flattened sand module and the cliff module contributes to the
	// final elevation.  This causes sand to appear at the low elevations since
	// the sand is slightly higher than the cliff base.
	//
	// -1.0 represents the lowest elevations and +1.0 represents the highest
	// elevations.
	//

	// 1: [Scaled-sand-dunes module]: This scale/bias module considerably
	//    flattens the output value from the badlands-sands subgroup and lowers
	//    this value to near -1.0.
	float badlandsTerrain_sb = badlandsSand_ad * 0.25 -0.75;

	// 2: [Dunes-and-cliffs module]: This maximum-value module causes the dunes
	//    to appear in the low areas and the cliffs to appear in the high areas.
	//    It does this by selecting the maximum of the output values from the
	//    scaled-sand-dunes module and the badlands-cliffs subgroup.
	float badlandsTerrain_ma = max(badlandsCliffs_te, badlandsTerrain_sb);
	
	float n = badlandsTerrain_ma;
//*/
	vec3 temp = packHeight(n * 0.5 + 0.5);
	gl_FragColor = vec4(temp, n * 0.5+0.5);

	
}
