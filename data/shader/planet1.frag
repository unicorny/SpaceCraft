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
varying vec4 c_color;
float f_lacunarity;
float f_persistence;

vec4 permute(vec4 x) {return mod(((x*34.0)+1.0)*x, 289.0);}
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

float select(float low, float high, float select, float bound, float edgeFallOff)
{
	//return select;
	float s = 0.5+0.5*select;
	float alpha = SCurve3((select-bound+edgeFallOff)/(2.0*edgeFallOff));	

	if(select < bound - edgeFallOff) return low;
	else if(select < bound + edgeFallOff) return LinearInterp(low, high, alpha);
	else return high;
}

float turbulence(vec3 p, float value, float frequenzy, float power)
{
	vec3 v0, v1, v2;
	v0 = p + vec3(12414.0, 65124.0, 31337.0) / 65536.0;
    v1 = p + vec3(26519.0, 18128.0, 60493.0) / 65536.0;
  	v2 = p + vec3(53820.0, 11213.0, 44845.0) / 65536.0;
	vec3 distort = p + vec3(sOctaveNoise(v0, frequenzy, 6), 
							sOctaveNoise(v1, frequenzy, 6),
							sOctaveNoise(v2, frequenzy, 6)) * power;
  // Retrieve the output value at the offsetted input value instead of the
  // original input value.
    //return m_pSourceModule[0]->GetValue (xDistort, yDistort, zDistort);
	return value;
}

vec4 gradientColor(float value, vec4 points[10], int pointsCount)
{
  // Find the first element in the control point array that has a value
  // larger than the output value from the source module.
  int indexPos;
  for (indexPos = 0; indexPos < pointsCount; indexPos++) {
    if (value < points[indexPos].w) {
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
    return vec4(points[index.y].rgb, 1.0);

  // Compute the alpha value used for linear interpolation.
  vec2 inputv = vec2(points[index.x].w, points[index.y].w);
  float alpha = (value - inputv.x) / (inputv.y - inputv.x);

  vec4 color0 = vec4(points[index.x].rgb, 1.0);
  vec4 color1 = vec4(points[index.y].rgb, 1.0);

  // Now perform the linear interpolation given the alpha value.
  return LinearInterpColor (color0, color1, alpha);
}

void main( void )
{
	//Default
	f_lacunarity = 2.0;
	f_persistence = 0.5;
/*  float n = snoise(v_texCoord3D*1.0);
  n += snoise(v_texCoord3D*2.0)*0.5;
  n += snoise(v_texCoord3D*4.0)*0.5;
  n += cridged(v_texCoord3D*32.0, 16.0, 14.0)*(0.6-n);
  n += snoise(v_texCoord3D*64.0);
  n += cnoise(v_texCoord3D*128.0);
  n += cnoise(v_texCoord3D*256.0);
  n += cnoise(v_texCoord3D*512.0);
  n += sridged(v_texCoord3D*32.0, 16.0)*(0.6-n);
*/
   float mountainTerrain = sridged(v_texCoord3D, 4.0, 15);
   //n = cOctaveNoise(v_texCoord3D, 4.0, 4);
   float baseFlatTerrain = sbillow(v_texCoord3D, 2.0, 14);
   float flatTerrain = baseFlatTerrain * 0.125-0.75;
	
   float oldPersistence = f_persistence;
   f_persistence = 0.25;
   float terrainType = sOctaveNoise(v_texCoord3D, 1, 6);
   f_persistence = oldPersistence;

   float terrainSelector = select(flatTerrain, mountainTerrain, terrainType, 0.0, 0.125);
   float n = terrainSelector;//turbulence(v_texCoord3D, terrainSelector, 4, 0.125);
   float mSeaLevelInMetres = 0.0;
   gl_FragColor = vec4(0.5 + 0.5*vec3(n, n, n), 1.0);
	vec4 gradient[10];
	gradient[0] =  vec4(vec3(0), -2.0 + mSeaLevelInMetres);
    gradient[1] =  vec4(vec3(6, 58, 127)/255.0, -0.03125 + mSeaLevelInMetres);
    gradient[2] =  vec4(vec3( 14, 112, 192)/255.0,  -0.0001220703 + mSeaLevelInMetres);
    gradient[3] =  vec4(vec3(70, 120,  60)/255.0, 0.0 + mSeaLevelInMetres);
    gradient[4] =  vec4(vec3(110, 140,  75)/255.0, 0.125 + mSeaLevelInMetres);
    gradient[5] =  vec4(vec3(160, 140, 111)/255.0, 0.25 + mSeaLevelInMetres);
    gradient[6] =  vec4(vec3(184, 163, 141)/255.0, 0.375 + mSeaLevelInMetres);
    gradient[7] =  vec4(vec3(1.0),                 0.5 + mSeaLevelInMetres);
    gradient[8] =  vec4(vec3(0.5, 1.0, 1.0),  0.75 + mSeaLevelInMetres);
    gradient[9] =  vec4(vec3(0.0,0.0,1.0), 2.0 + mSeaLevelInMetres);

   	//gl_FragColor = gradientColor(n, gradient, 10);//vec4(0.5 + 0.5*vec3(n, n, n), 1.0);

//	vec4 color = c_color+n*0.2;
  //  gl_FragColor = vec4(0.5 + 0.5 * vec3(color.x+0.1, color.y+0.2, color.z*color.z), 0.0);

//  vec4 color = texture2D(texture, gl_TexCoord[0].st);
//  gl_FragColor = color;
//  gl_FragColor = mix(texture2D(texture, gl_TexCoord[0].st),
//                     vec4(1.0, 1.0, 1.0, 1.0), 0);//(0.5+0.5*n)*0.2);
	//vec4 color = gl_Color;
  //mix(gl_Color,
//                     vec4(1.0, 1.0, 1.0, 1.0), 0.0);//(0.5+0.5*n)*0.2);
}
