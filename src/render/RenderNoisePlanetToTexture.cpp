#include "RenderNoisePlanetToTexture.h"
#include "ShaderManager.h"
#include "DRGeometrieManager.h"

RenderNoisePlanetToTexture::RenderNoisePlanetToTexture(const char* vertexShaderName, const char* fragmentShaderName)
: RenderInStepsToTexture(), mShader(NULL), mRenderSphere(NULL)
{
    if(!vertexShaderName || !fragmentShaderName)
        LOG_ERROR_VOID("Fehler kein Shader");
	
    mShader = ShaderManager::Instance().getShader(vertexShaderName, fragmentShaderName);    
    mRenderSphere = DRGeometrieManager::Instance().getGrid(50, GEO_FULL);
       
}

RenderNoisePlanetToTexture::~RenderNoisePlanetToTexture()
{
    ShaderManager::Instance().releaseShader(mShader);
    DRGeometrieManager::Instance().freeGrid(50, GEO_FULL);
}

DRReturn RenderNoisePlanetToTexture::renderStuff()
{
    mShader->bind();
    
    mShader->setUniform3fv("SphericalCenter", DRVector3(0.0f, 0.0f, -1.0f*(1.0f-mH)));
	mShader->setUniform1f("theta", mTheta);
    
    //printf("theta:%f\n", 90.0*GRADTORAD);
    
    // Frequency of the planet's continents.  Higher frequency produces smaller,
    // more numerous continents.  This value is measured in radians.
	mShader->setUniform1f("CONTINENT_FREQUENCY", 1.0f);
    
    // Lacunarity of the planet's continents.  Changing this value produces
    // slightly different continents.  For the best results, this value should
    // be random, but close to 2.0.
	mShader->setUniform1f("CONTINENT_LACUNARITY", 2.208984375f);
    
    // Lacunarity of the planet's mountains.  Changing this value produces
    // slightly different mountains.  For the best results, this value should
    // be random, but close to 2.0.
	mShader->setUniform1f("MOUNTAIN_LACUNARITY", 2.142578125f);
    
    // Lacunarity of the planet's hills.  Changing this value produces slightly
    // different hills.  For the best results, this value should be random, but
    // close to 2.0.
	mShader->setUniform1f("HILLS_LACUNARITY", 2.162109375f);
    
    // Lacunarity of the planet's plains.  Changing this value produces slightly
    // different plains.  For the best results, this value should be random, but
    // close to 2.0.
	mShader->setUniform1f("PLAINS_LACUNARITY", 2.314453125f);
    
    // Lacunarity of the planet's badlands.  Changing this value produces
    // slightly different badlands.  For the best results, this value should be
    // random, but close to 2.0.
	mShader->setUniform1f("BADLANDS_LACUNARITY", 2.212890625f);
    
    // Specifies the amount of "glaciation" on the mountains.  This value
    // should be close to 1.0 and greater than 1.0.
	mShader->setUniform1f("MOUNTAIN_GLACIATION", 1.375);
 
    // Specifies the planet's sea level.  This value must be between -1.0
    // (minimum planet elevation) and +1.0 (maximum planet elevation.)
    const float SEA_LEVEL = 0.0f;
	mShader->setUniform1f("SEA_LEVEL", SEA_LEVEL);
    
    // Specifies the level on the planet in which continental shelves appear.
    // This value must be between -1.0 (minimum planet elevation) and +1.0
    // (maximum planet elevation), and must be less than SEA_LEVEL.
	mShader->setUniform1f("SHELF_LEVEL", -0.375f);
    
    // Determines the amount of mountainous terrain that appears on the
    // planet.  Values range from 0.0 (no mountains) to 1.0 (all terrain is
    // covered in mountains).  Mountainous terrain will overlap hilly terrain.
    // Because the badlands terrain may overlap parts of the mountainous
    // terrain, setting MOUNTAINS_AMOUNT to 1.0 may not completely cover the
    // terrain in mountains.
    const float MOUNTAINS_AMOUNT = 0.5f;
	mShader->setUniform1f("MOUNTAINS_AMOUNT", MOUNTAINS_AMOUNT);
  
    // Determines the amount of hilly terrain that appears on the planet.
    // Values range from 0.0 (no hills) to 1.0 (all terrain is covered in
    // hills).  This value must be less than MOUNTAINS_AMOUNT.  Because the
    // mountainous terrain will overlap parts of the hilly terrain, and
    // the badlands terrain may overlap parts of the hilly terrain, setting
    // HILLS_AMOUNT to 1.0 may not completely cover the terrain in hills.
	mShader->setUniform1f("HILLS_AMOUNT", (1.0f + MOUNTAINS_AMOUNT) / 2.0f);
    
    // Determines the amount of badlands terrain that covers the planet.
    // Values range from 0.0 (no badlands) to 1.0 (all terrain is covered in
    // badlands.)  Badlands terrain will overlap any other type of terrain.
	mShader->setUniform1f("BADLANDS_AMOUNT", 0.03125f);
    
    // Maximum depth of the rivers, in planetary elevation units.
	mShader->setUniform1f("RIVER_DEPTH", 0.0234375f);
    
    // Scaling to apply to the base continent elevations, in planetary elevation
    // units.
	mShader->setUniform1f("CONTINENT_HEIGHT_SCALE", (1.0f - SEA_LEVEL) / 4.0f);
    
    // Calculate the sea level, in meters.
    float MAX_ELEV = 1.0f; float MIN_ELEV = -1.0f;
    float seaLevelInMeters = (((SEA_LEVEL + 1.0f) / 2.0f)
    * (MAX_ELEV - MIN_ELEV)) + MIN_ELEV;
	mShader->setUniform1f("SEA_LEVEL_IN_METRES", seaLevelInMeters);
    //printf("seaLevelInMeters: %f\n", seaLevelInMeters);

	mShader->setUniformMatrix("projection", mProjectionMatrix);
	mShader->setUniformMatrix("texture", mRotation);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    mRenderSphere->render();    
    
    mShader->unbind();
    return DR_OK;
}

DRReturn RenderNoisePlanetToTexture::init(float stepSize, float theta, float h, Texture* texture, const DRMatrix& rotation /*= DRMatrix::Identity()*/)
{
    mTheta = theta;
    mH = h;
    mRotation = rotation;
    float clippingPlanes[4] = {1.0f, -1.0f, 1.0f, -1.0f};
    
    return RenderInStepsToTexture::init(stepSize, clippingPlanes, texture);    
}
