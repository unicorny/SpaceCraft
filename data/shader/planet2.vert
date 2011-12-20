#version 120

uniform float time;
/*
 * Both 2D, 3D and 4D texture coordinates are defined, for testing purposes.
 */
varying vec2 v_texCoord2D;
varying vec3 v_texCoord3D;
varying vec4 c_color;

varying float CONTINENT_FREQUENCY;
varying float CONTINENT_LACUNARITY;
varying float MOUNTAIN_LACUNARITY;
varying float HILLS_LACUNARITY;
varying float PLAINS_LACUNARITY;
varying float BADLANDS_LACUNARITY;
varying float MOUNTAIN_GLACIATION;
varying float SEA_LEVEL;
varying float SHELF_LEVEL;
varying float CONTINENT_HEIGHT_SCALE;
varying float HILLS_AMOUNT;
varying float MOUNTAINS_AMOUNT;
varying float BADLANDS_AMOUNT;
varying float RIVER_DEPTH;

varying float mSeaLevelInMetres;

const float PI = 3.1415926535;

void main( void )
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
    vec4 pos = gl_Vertex;
	vec2 tex;
	tex.x = acos(pos.z) / PI;
	if(pos.y > 0.0)
		tex.y = acos(pos.x/(sin(PI*tex.x))) / (2.0*PI);
	else
		tex.y = (PI + acos(pos.x/(sin(PI*tex.x)))) / (2.0*PI);
//	v_texCoord2D = gl_MultiTexCoord0.xy * 16.0;
    v_texCoord2D = gl_MultiTexCoord0.xy;

	c_color = gl_Color;

	const int seed =182;
    v_texCoord3D = normalize(gl_Vertex).xyz + seed; 
//	v_texCoord3D = vec3(gl_MultiTexCoord0.xy, seed);

// Frequency of the planet's continents.  Higher frequency produces smaller,
	// more numerous continents.  This value is measured in radians.
	CONTINENT_FREQUENCY = 1.0;

	// Lacunarity of the planet's continents.  Changing this value produces
	// slightly different continents.  For the best results, this value should
	// be random, but close to 2.0.
	CONTINENT_LACUNARITY = 2.208984375;

	// Lacunarity of the planet's mountains.  Changing this value produces
	// slightly different mountains.  For the best results, this value should
	// be random, but close to 2.0.
	MOUNTAIN_LACUNARITY = 2.142578125;

	// Lacunarity of the planet's hills.  Changing this value produces slightly
	// different hills.  For the best results, this value should be random, but
	// close to 2.0.
	HILLS_LACUNARITY = 2.162109375;

	// Lacunarity of the planet's plains.  Changing this value produces slightly
	// different plains.  For the best results, this value should be random, but
	// close to 2.0.
	PLAINS_LACUNARITY = 2.314453125;

	// Lacunarity of the planet's badlands.  Changing this value produces
	// slightly different badlands.  For the best results, this value should be
	// random, but close to 2.0.
	BADLANDS_LACUNARITY = 2.212890625;

	// Specifies the amount of "glaciation" on the mountains.  This value
	// should be close to 1.0 and greater than 1.0.
	MOUNTAIN_GLACIATION = 1.375;

	// Specifies the planet's sea level.  This value must be between -1.0
	// (minimum planet elevation) and +1.0 (maximum planet elevation.)
 	SEA_LEVEL = 0.0;

	// Specifies the level on the planet in which continental shelves appear.
	// This value must be between -1.0 (minimum planet elevation) and +1.0
	// (maximum planet elevation), and must be less than SEA_LEVEL.
	SHELF_LEVEL = -0.375;

	// Scaling to apply to the base continent elevations, in planetary elevation
	// units.
	CONTINENT_HEIGHT_SCALE = (1.0 - SEA_LEVEL) / 4.0;

	// Determines the amount of mountainous terrain that appears on the
	// planet.  Values range from 0.0 (no mountains) to 1.0 (all terrain is
	// covered in mountains).  Mountainous terrain will overlap hilly terrain.
	// Because the badlands terrain may overlap parts of the mountainous
	// terrain, setting MOUNTAINS_AMOUNT to 1.0 may not completely cover the
	// terrain in mountains.
	MOUNTAINS_AMOUNT = 0.5;

	// Determines the amount of hilly terrain that appears on the planet.
	// Values range from 0.0 (no hills) to 1.0 (all terrain is covered in
	// hills).  This value must be less than MOUNTAINS_AMOUNT.  Because the
	// mountainous terrain will overlap parts of the hilly terrain, and
	// the badlands terrain may overlap parts of the hilly terrain, setting
	// HILLS_AMOUNT to 1.0 may not completely cover the terrain in hills.
	HILLS_AMOUNT = (1.0 + MOUNTAINS_AMOUNT) / 2.0;

	// Determines the amount of badlands terrain that covers the planet.
	// Values range from 0.0 (no badlands) to 1.0 (all terrain is covered in
	// badlands.)  Badlands terrain will overlap any other type of terrain.
	BADLANDS_AMOUNT = 0.03125;

	// Maximum depth of the rivers, in planetary elevation units.
	RIVER_DEPTH = 0.0234375;



	float MAX_ELEV = 1.0, MIN_ELEV = -1.0;
	mSeaLevelInMetres = (((SEA_LEVEL + 1.0) / 2.0)
		* (MAX_ELEV - MIN_ELEV)) + MIN_ELEV;

}

// simple vertex shader

/*void main()
{
	gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor  = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
*/
