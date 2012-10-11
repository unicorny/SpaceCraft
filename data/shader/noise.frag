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

uniform sampler2D texture;

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

struct GradientColor
{
	float pos;
	vec4 color;
};

//libnoise helper
float SCurve3 (float a) {return (a * a * (3.0 - 2.0 * a));}
float LinearInterp (float n0, float n1, float a) {return ((1.0 - a) * n0) + (a * n1);}
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
// noise helper
vec4 permute(vec4 x) {return mod(((x*34.0)+1.0)*x, 289.0);}
vec4 taylorInvSqrt(vec4 r) {return 1.79284291400159 - 0.85373472095314 * r;}
vec3 fade(vec3 t) {return t*t*t*(t*(t*6.0-15.0)+10.0);}

float snoise(vec3 v)
  { 
  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

// First corner
  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;

// Other corners
  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );

  //  x0 = x0 - 0. + 0.0 * C 
  vec3 x1 = x0 - i1 + 1.0 * C.xxx;
  vec3 x2 = x0 - i2 + 2.0 * C.xxx;
  vec3 x3 = x0 - 1. + 3.0 * C.xxx;

// Permutations
  i = mod(i, 289.0 ); 
  vec4 p = permute( permute( permute( 
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) 
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

// Gradients
// ( N*N points uniformly over a square, mapped onto an octahedron.)
  float n_ = 1.0/7.0; // N=7
  vec3  ns = n_ * D.wyz - D.xzx;

  vec4 j = p - 49.0 * floor(p * ns.z *ns.z);  //  mod(p,N*N)

  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);

  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );

  //vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
  //vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

//Normalise gradients
  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

// Mix final noise value
  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m;
  return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), 
                                dot(p2,x2), dot(p3,x3) ) );
  }

// Classic Perlin noise
float cnoise(vec3 P)
{
  vec3 Pi0 = floor(P); // Integer part for indexing
  vec3 Pi1 = Pi0 + vec3(1.0); // Integer part + 1
  Pi0 = mod(Pi0, 289.0);
  Pi1 = mod(Pi1, 289.0);
  vec3 Pf0 = fract(P); // Fractional part for interpolation
  vec3 Pf1 = Pf0 - vec3(1.0); // Fractional part - 1.0
  vec4 ix = vec4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
  vec4 iy = vec4(Pi0.yy, Pi1.yy);
  vec4 iz0 = Pi0.zzzz;
  vec4 iz1 = Pi1.zzzz;

  vec4 ixy = permute(permute(ix) + iy);
  vec4 ixy0 = permute(ixy + iz0);
  vec4 ixy1 = permute(ixy + iz1);

  vec4 gx0 = ixy0 / 7.0;
  vec4 gy0 = fract(floor(gx0) / 7.0) - 0.5;
  gx0 = fract(gx0);
  vec4 gz0 = vec4(0.5) - abs(gx0) - abs(gy0);
  vec4 sz0 = step(gz0, vec4(0.0));
  gx0 -= sz0 * (step(0.0, gx0) - 0.5);
  gy0 -= sz0 * (step(0.0, gy0) - 0.5);

  vec4 gx1 = ixy1 / 7.0;
  vec4 gy1 = fract(floor(gx1) / 7.0) - 0.5;
  gx1 = fract(gx1);
  vec4 gz1 = vec4(0.5) - abs(gx1) - abs(gy1);
  vec4 sz1 = step(gz1, vec4(0.0));
  gx1 -= sz1 * (step(0.0, gx1) - 0.5);
  gy1 -= sz1 * (step(0.0, gy1) - 0.5);

  vec3 g000 = vec3(gx0.x,gy0.x,gz0.x);
  vec3 g100 = vec3(gx0.y,gy0.y,gz0.y);
  vec3 g010 = vec3(gx0.z,gy0.z,gz0.z);
  vec3 g110 = vec3(gx0.w,gy0.w,gz0.w);
  vec3 g001 = vec3(gx1.x,gy1.x,gz1.x);
  vec3 g101 = vec3(gx1.y,gy1.y,gz1.y);
  vec3 g011 = vec3(gx1.z,gy1.z,gz1.z);
  vec3 g111 = vec3(gx1.w,gy1.w,gz1.w);

  vec4 norm0 = taylorInvSqrt(vec4(dot(g000, g000), dot(g010, g010), dot(g100, g100), dot(g110, g110)));
  g000 *= norm0.x;
  g010 *= norm0.y;
  g100 *= norm0.z;
  g110 *= norm0.w;
  vec4 norm1 = taylorInvSqrt(vec4(dot(g001, g001), dot(g011, g011), dot(g101, g101), dot(g111, g111)));
  g001 *= norm1.x;
  g011 *= norm1.y;
  g101 *= norm1.z;
  g111 *= norm1.w;

  float n000 = dot(g000, Pf0);
  float n100 = dot(g100, vec3(Pf1.x, Pf0.yz));
  float n010 = dot(g010, vec3(Pf0.x, Pf1.y, Pf0.z));
  float n110 = dot(g110, vec3(Pf1.xy, Pf0.z));
  float n001 = dot(g001, vec3(Pf0.xy, Pf1.z));
  float n101 = dot(g101, vec3(Pf1.x, Pf0.y, Pf1.z));
  float n011 = dot(g011, vec3(Pf0.x, Pf1.yz));
  float n111 = dot(g111, Pf1);

  vec3 fade_xyz = fade(Pf0);
  vec4 n_z = mix(vec4(n000, n100, n010, n110), vec4(n001, n101, n011, n111), fade_xyz.z);
  vec2 n_yz = mix(n_z.xy, n_z.zw, fade_xyz.y);
  float n_xyz = mix(n_yz.x, n_yz.y, fade_xyz.x); 
  return 2.2 * n_xyz;
}
/// Square root of 3.
const float SQRT_3 = 1.7320508075688772935;
#extension GL_EXT_gpu_shader4 : enable
int IntValueNoise3D (ivec3 p, int seed)
{
  // All constants are primes and must remain prime in order for this noise
  // function to work correctly.
  int n = (
      1619    * p.x
    + 31337    * p.y
    + 6971    * p.z
    + 1013 * seed)
    & 0x7fffffff;
  n = (n >> 13) ^ n;
  return (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
}

float ValueNoise3D (vec3 p, int seed)
{
  return 1.0 - int(IntValueNoise3D (ivec3(p), seed) / 1073741824.0);
}

float voronoi(vec3 p, int seed, float frequenzy, float displacement, bool enableDistance)
{
	// This method could be more efficient by caching the seed values.  Fix
  // later.

  p *= frequenzy;
  
  ivec3 Int = ivec3(p.x > 0.0? int(p.x): int(p.x) - 1,
					p.y > 0.0? int(p.y): int(p.y) - 1,
					p.z > 0.0? int(p.z): int(p.z) - 1);

  float minDist = 2147483647.0;
  vec3 candidate = vec3(0.0);

  // Inside each unit cube, there is a seed point at a random position.  Go
  // through each of the nearby cubes until we find a cube with a seed point
  // that is closest to the specified position.
  for (int zCur = Int.z - 2; zCur <= Int.z + 2; zCur++) {
    for (int yCur = Int.y - 2; yCur <= Int.y + 2; yCur++) {
      for (int xCur = Int.x - 2; xCur <= Int.x + 2; xCur++) {

        // Calculate the position and distance to the seed point inside of
        // this unit cube.
		vec3 Pos = vec3(xCur + ValueNoise3D (vec3(xCur, yCur, zCur), seed    ),	
						yCur + ValueNoise3D (vec3(xCur, yCur, zCur), seed + 1),
						zCur + ValueNoise3D (vec3(xCur, yCur, zCur), seed + 2));
		vec3 Dist = Pos-p;
        float dist = Dist.x * Dist.x + Dist.y * Dist.y + Dist.z * Dist.z;

        if (dist < minDist) {
          // This seed point is closer to any others found so far, so record
          // this seed point.
          minDist = dist;
		  candidate = Pos;
        }
      }
    }
  }

  float value;
  if (enableDistance)
  {
    // Determine the distance to the nearest seed point.
	vec3 Dist = candidate - p;
    value = (length(Dist)) * SQRT_3 - 1.0;
  } 
  else
  {
    value = 0.0;
  }

  // Return the calculated distance with the displacement value applied.
  return value + (displacement * ValueNoise3D (vec3(floor (candidate)), seed));
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
				float noise = 0.5 + 0.5 * snoise(i+index);
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
		value += snoise(p*frequenzy) * curPersistence;
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
		signal = snoise(p);

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
		signal = snoise(p);
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
	
	//return BaseContinentDefinition(value);
	
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

void main( void )
{
	//Default
	const float DEFAULT_LACUNARTITY = 2.0;

	f_lacunarity = DEFAULT_LACUNARTITY;
	f_persistence = 0.5;

	vec2 curveTemp[10];
	float terraceTemp[6];
	float n = 0;

	f_lacunarity = CONTINENT_LACUNARITY;
	float baseContinentDef_pe0 = sOctaveNoise(v_texCoord3D, CONTINENT_FREQUENCY, 14);
//n = baseContinentDef_pe0;

	//turbulences must be calculated bevore
	

	//Module subgroup: terrain type definition (3 noise modules)
	// 1: [Warped-continent module]: This turbulence module slightly warps the
  	//    output value from the continent-definition group.  This prevents the
  	//    rougher terrain from appearing exclusively at higher elevations.
  	//    Rough areas may now appear in the the ocean, creating rocky islands
  	//    and fjords.
	vec3 terrainTypeDef_tu = turbulence(v_texCoord3D, 20, CONTINENT_FREQUENCY * 18.125, CONTINENT_FREQUENCY / 20.59375 * TERRAIN_OFFSET, 3);
	//terrainTypeDef_tu = v_texCoord3D;
  
	float continentDef_se = BaseContinentDefinition_se(v_texCoord3D);
	float continentDef_se_tu = BaseContinentDefinition_se(terrainTypeDef_tu);

	////////////////////////////////////////////////////////////////////////////
	// Module group: terrain type definition
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	// Module subgroup: terrain type definition (3 noise modules)
	//
	// This subgroup defines the positions of the terrain types on the planet.
	//
	// Terrain types include, in order of increasing roughness, plains, hills,
	// and mountains.
	//
	// This subgroup's output value is based on the output value from the
	// continent-definition group.  Rougher terrain mainly appears at higher
	// elevations.
	//
	// -1.0 represents the smoothest terrain types (plains and underwater) and
	// +1.0 represents the roughest terrain types (mountains).
	//
	


	// 2: [Roughness-probability-shift module]: This terracing module sharpens
	//    the edges of the warped-continent module near sea level and lowers
	//    the slope towards the higher-elevation areas.  This shrinks the areas
	//    in which the rough terrain appears, increasing the "rarity" of rough
	//    terrain.
	//terrainTypeDef_te.SetSourceModule (0, terrainTypeDef_tu);
	terraceTemp[0] = -1.0;
	terraceTemp[1] = SHELF_LEVEL + SEA_LEVEL / 2.0;
	terraceTemp[2] =  1.0;
	float terrainTypeDef_te = terrace(continentDef_se_tu, terraceTemp, 3);
	
	
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

	


	if(n == 0.0)
	   	n = baseContinentElev_se;
//*/
	vec3 temp = packHeight(n * 0.5 + 0.5);
	
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

   	gl_FragColor = gradientColor(n, gradient, 10);//vec4(0.5 + 0.5*vec3(n, n, n), 1.0);
	n = unpackHeight(temp);
	gl_FragColor.w = n * 0.5 + 0.5;
        //gl_FragColor = vec4(0.5 + 0.5*vec3(n,n,n), n*0.5+0.5);
	vec4 color = vec4(v_texCoord3D, 1.0);
	gl_FragColor = vec4(temp, n * 0.5+0.5);
	//gl_FragColor = color;
	
}
