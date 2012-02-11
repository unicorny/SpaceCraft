#include "main.h"
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
    int sphereCenterLocation = glGetUniformLocation(mShader->getProgram(), "SphericalCenter");
    int thetaLocation = glGetUniformLocation(mShader->getProgram(), "theta");
    int continent_frequenzy_Location = glGetUniformLocation(mShader->getProgram(), "CONTINENT_FREQUENCY");
    int continent_lacunarity_Location = glGetUniformLocation(mShader->getProgram(), "CONTINENT_LACUNARITY");
    int mountain_lacunarity_Location = glGetUniformLocation(mShader->getProgram(), "MOUNTAIN_LACUNARITY");
    int hills_lacunarity_Location = glGetUniformLocation(mShader->getProgram(), "HILLS_LACUNARITY");
    int plains_lacunarity_Location = glGetUniformLocation(mShader->getProgram(), "PLAINS_LACUNARITY");
    int badlands_lacunarity_Location = glGetUniformLocation(mShader->getProgram(), "BADLANDS_LACUNARITY");
    int mountain_glaciation_Location = glGetUniformLocation(mShader->getProgram(), "MOUNTAIN_GLACIATION");
    int sea_level_Location = glGetUniformLocation(mShader->getProgram(), "SEA_LEVEL");
    int shelf_level_Location = glGetUniformLocation(mShader->getProgram(), "SHELF_LEVEL");
    int hills_amount_Location = glGetUniformLocation(mShader->getProgram(), "HILLS_AMOUNT");
    int mountains_amount_Location = glGetUniformLocation(mShader->getProgram(), "MOUNTAINS_AMOUNT");
    int badlands_amount_Location = glGetUniformLocation(mShader->getProgram(), "BADLANDS_AMOUNT");
    int river_depth_Location = glGetUniformLocation(mShader->getProgram(), "RIVER_DEPTH");
    int continent_height_scale_Location = glGetUniformLocation(mShader->getProgram(), "CONTINENT_HEIGHT_SCALE");
    int sea_level_in_metres_Location = glGetUniformLocation(mShader->getProgram(), "SEA_LEVEL_IN_METRES");

    glUniform3fv(sphereCenterLocation, 1, static_cast<float*>(DRVector3(0.0f, 0.0f, -1.0f*(1.0f-mH))));
    glUniform1f(thetaLocation, static_cast<float>(mTheta));
    
    //printf("theta:%f\n", 90.0*GRADTORAD);
    
    // Frequency of the planet's continents.  Higher frequency produces smaller,
    // more numerous continents.  This value is measured in radians.
    glUniform1f(continent_frequenzy_Location, 1.0f);
    
    // Lacunarity of the planet's continents.  Changing this value produces
    // slightly different continents.  For the best results, this value should
    // be random, but close to 2.0.
    glUniform1f(continent_lacunarity_Location, 2.208984375f);
    
    // Lacunarity of the planet's mountains.  Changing this value produces
    // slightly different mountains.  For the best results, this value should
    // be random, but close to 2.0.
    glUniform1f(mountain_lacunarity_Location, 2.142578125f);
    
    // Lacunarity of the planet's hills.  Changing this value produces slightly
    // different hills.  For the best results, this value should be random, but
    // close to 2.0.
    glUniform1f(hills_lacunarity_Location, 2.162109375f);
    
    // Lacunarity of the planet's plains.  Changing this value produces slightly
    // different plains.  For the best results, this value should be random, but
    // close to 2.0.
    glUniform1f(plains_lacunarity_Location, 2.314453125f);
    
    // Lacunarity of the planet's badlands.  Changing this value produces
    // slightly different badlands.  For the best results, this value should be
    // random, but close to 2.0.
    glUniform1f(badlands_lacunarity_Location, 2.212890625f);
    
    // Specifies the amount of "glaciation" on the mountains.  This value
    // should be close to 1.0 and greater than 1.0.
    glUniform1f(mountain_glaciation_Location, 1.375);
 
    // Specifies the planet's sea level.  This value must be between -1.0
    // (minimum planet elevation) and +1.0 (maximum planet elevation.)
    const float SEA_LEVEL = 0.0f;
    glUniform1f(sea_level_Location, SEA_LEVEL);
    
    // Specifies the level on the planet in which continental shelves appear.
    // This value must be between -1.0 (minimum planet elevation) and +1.0
    // (maximum planet elevation), and must be less than SEA_LEVEL.
    glUniform1f(shelf_level_Location, -0.375f);
    
    // Determines the amount of mountainous terrain that appears on the
    // planet.  Values range from 0.0 (no mountains) to 1.0 (all terrain is
    // covered in mountains).  Mountainous terrain will overlap hilly terrain.
    // Because the badlands terrain may overlap parts of the mountainous
    // terrain, setting MOUNTAINS_AMOUNT to 1.0 may not completely cover the
    // terrain in mountains.
    const float MOUNTAINS_AMOUNT = 0.5f;
    glUniform1f(mountains_amount_Location, MOUNTAINS_AMOUNT);
  
    // Determines the amount of hilly terrain that appears on the planet.
    // Values range from 0.0 (no hills) to 1.0 (all terrain is covered in
    // hills).  This value must be less than MOUNTAINS_AMOUNT.  Because the
    // mountainous terrain will overlap parts of the hilly terrain, and
    // the badlands terrain may overlap parts of the hilly terrain, setting
    // HILLS_AMOUNT to 1.0 may not completely cover the terrain in hills.
    glUniform1f(hills_amount_Location, (1.0f + MOUNTAINS_AMOUNT) / 2.0f);
    
    // Determines the amount of badlands terrain that covers the planet.
    // Values range from 0.0 (no badlands) to 1.0 (all terrain is covered in
    // badlands.)  Badlands terrain will overlap any other type of terrain.
    glUniform1f(badlands_amount_Location, 0.03125f);
    
    // Maximum depth of the rivers, in planetary elevation units.
    glUniform1f(river_depth_Location, 0.0234375f);
    
    // Scaling to apply to the base continent elevations, in planetary elevation
    // units.
    glUniform1f(continent_height_scale_Location, (1.0f - SEA_LEVEL) / 4.0f);    
    
    // Calculate the sea level, in meters.
    float MAX_ELEV = 1.0f; float MIN_ELEV = -1.0f;
    float seaLevelInMeters = (((SEA_LEVEL + 1.0) / 2.0)
    * (MAX_ELEV - MIN_ELEV)) + MIN_ELEV;
    glUniform1f(sea_level_in_metres_Location, seaLevelInMeters);    
    //printf("seaLevelInMeters: %f\n", seaLevelInMeters);

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    
    glMultMatrixf(mRotation);
    
    mRenderSphere->render();    
    
    mShader->unbind();
    return DR_OK;
}

DRReturn RenderNoisePlanetToTexture::init(float stepSize, float theta, float h, GLuint textureID, const DRMatrix& rotation /*= DRMatrix::Identity()*/)
{
    mTheta = theta;
    mH = h;
    mRotation = rotation;
    float clippingPlanes[4] = {1.0f, -1.0f, 1.0f, -1.0f};
    
    return RenderInStepsToTexture::init(stepSize, clippingPlanes, textureID);    
}
