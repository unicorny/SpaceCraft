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

uniform float BADLANDS_LACUNARITY;

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

	float n = badlandsSand_ad;
//*/
	vec3 temp = packHeight(n * 0.5 + 0.5);
	gl_FragColor = vec4(temp, n * 0.5+0.5);

	
}
