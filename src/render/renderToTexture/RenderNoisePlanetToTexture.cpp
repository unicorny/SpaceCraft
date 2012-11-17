#include "RenderNoisePlanetToTexture.h"
#include "ShaderManager.h"
#include "DRGeometrieManager.h"
#include "PlanetSektor.h"


RenderNoisePlanetToTexture::RenderNoisePlanetToTexture(ShaderProgramParameter shader[5], const PlanetNoiseParameter* noiseParameter)
: RenderInStepsToTexture(), mShaderCursor(0), mRenderGrid(NULL), mNoiseParameter(noiseParameter)
{
    for(int i = 0; i < 5; i++)
        mShader[i] = ShaderManager::Instance().getShaderProgram(&shader[i]);    
    mRenderGrid = DRGeometrieManager::Instance().getGrid(50, GEO_FULL, GEO_VERTEX_TRIANGLE_STRIP);
    mPermTexture = DRTextureManager::Instance().getTexture("data/permTexture.tga", false, GL_NEAREST, GL_NEAREST);       
}


RenderNoisePlanetToTexture::~RenderNoisePlanetToTexture()
{
    DRGeometrieManager::Instance().freeGrid(50, GEO_FULL, GEO_VERTEX_TRIANGLE_STRIP);
}

DRReturn RenderNoisePlanetToTexture::renderStuff()
{
    if(!mNoiseParameter) LOG_ERROR("no noise parameter", DR_ERROR);
   
    mShader[mShaderCursor]->bind();
    
    if(3 == mShaderCursor)
    {
        glActiveTexture(GL_TEXTURE0);
        mPermTexture->bind();
        mShader[mShaderCursor]->setUniform1i("permTexture", 0);

        glActiveTexture(GL_TEXTURE1);
        maTextures[0]->bind();
        mShader[mShaderCursor]->setUniform1i("texture0", 1);
        
        glActiveTexture(GL_TEXTURE2);
        maTextures[1]->bind();
        mShader[mShaderCursor]->setUniform1i("texture1", 2);
        
        glActiveTexture(GL_TEXTURE3);
        maTextures[2]->bind();
        mShader[mShaderCursor]->setUniform1i("texture2", 3);
    }
    else if(4 == mShaderCursor)
    {
        glActiveTexture(GL_TEXTURE0);
        mPermTexture->bind();
        mShader[mShaderCursor]->setUniform1i("permTexture", 0);

        glActiveTexture(GL_TEXTURE1);
        maTextures[3]->bind();
        mShader[mShaderCursor]->setUniform1i("texture0", 1);
    }
    else
    {
        mPermTexture->bind();
    }
    
    //mShader->setUniform3fv("SphericalCenter", DRVector3(0.0f, 0.0f, -1.0f*(1.0f-mH)));
	mShader[mShaderCursor]->setUniform1f("patchScaling", mPatchScaling);
    mShader[mShaderCursor]->setUniform3fv("translate", mTranslate);
    
    //printf("theta:%f\n", 90.0*GRADTORAD);
    
    // Frequency of the planet's continents.  Higher frequency produces smaller,
    // more numerous continents.  This value is measured in radians.
	mShader[mShaderCursor]->setUniform1f("CONTINENT_FREQUENCY", mNoiseParameter->continentFrequenzy);
    
    // Lacunarity of the planet's continents.  Changing this value produces
    // slightly different continents.  For the best results, this value should
    // be random, but close to 2.0.
	mShader[mShaderCursor]->setUniform1f("CONTINENT_LACUNARITY", mNoiseParameter->continentLacunarity);
    
    // Lacunarity of the planet's mountains.  Changing this value produces
    // slightly different mountains.  For the best results, this value should
    // be random, but close to 2.0.
	mShader[mShaderCursor]->setUniform1f("MOUNTAIN_LACUNARITY", mNoiseParameter->mountainLacunarity);
    
    // Lacunarity of the planet's hills.  Changing this value produces slightly
    // different hills.  For the best results, this value should be random, but
    // close to 2.0.
	mShader[mShaderCursor]->setUniform1f("HILLS_LACUNARITY", mNoiseParameter->hillsLacunarity);
    
    // Lacunarity of the planet's plains.  Changing this value produces slightly
    // different plains.  For the best results, this value should be random, but
    // close to 2.0.
	mShader[mShaderCursor]->setUniform1f("PLAINS_LACUNARITY", mNoiseParameter->plainsLacunarity);
    
    // Lacunarity of the planet's badlands.  Changing this value produces
    // slightly different badlands.  For the best results, this value should be
    // random, but close to 2.0.
	mShader[mShaderCursor]->setUniform1f("BADLANDS_LACUNARITY", mNoiseParameter->badlandsLacunarity);
    
    // Specifies the amount of "glaciation" on the mountains.  This value
    // should be close to 1.0 and greater than 1.0.
	mShader[mShaderCursor]->setUniform1f("MOUNTAIN_GLACIATION", mNoiseParameter->mountainGlaciation);
 
    // Specifies the planet's sea level.  This value must be between -1.0
    // (minimum planet elevation) and +1.0 (maximum planet elevation.)
	mShader[mShaderCursor]->setUniform1f("SEA_LEVEL", mNoiseParameter->seaLevel);
    
    // Specifies the level on the planet in which continental shelves appear.
    // This value must be between -1.0 (minimum planet elevation) and +1.0
    // (maximum planet elevation), and must be less than SEA_LEVEL.
	mShader[mShaderCursor]->setUniform1f("SHELF_LEVEL", mNoiseParameter->shelfLevel);
    
    // Determines the amount of mountainous terrain that appears on the
    // planet.  Values range from 0.0 (no mountains) to 1.0 (all terrain is
    // covered in mountains).  Mountainous terrain will overlap hilly terrain.
    // Because the badlands terrain may overlap parts of the mountainous
    // terrain, setting MOUNTAINS_AMOUNT to 1.0 may not completely cover the
    // terrain in mountains.
	mShader[mShaderCursor]->setUniform1f("MOUNTAINS_AMOUNT", mNoiseParameter->mountainAmount);
  
    // Determines the amount of hilly terrain that appears on the planet.
    // Values range from 0.0 (no hills) to 1.0 (all terrain is covered in
    // hills).  This value must be less than MOUNTAINS_AMOUNT.  Because the
    // mountainous terrain will overlap parts of the hilly terrain, and
    // the badlands terrain may overlap parts of the hilly terrain, setting
    // HILLS_AMOUNT to 1.0 may not completely cover the terrain in hills.
	mShader[mShaderCursor]->setUniform1f("HILLS_AMOUNT", mNoiseParameter->hillsAmount);
    
    // Determines the amount of badlands terrain that covers the planet.
    // Values range from 0.0 (no badlands) to 1.0 (all terrain is covered in
    // badlands.)  Badlands terrain will overlap any other type of terrain.
	mShader[mShaderCursor]->setUniform1f("BADLANDS_AMOUNT", mNoiseParameter->badlandsAmount);
    
    // Maximum depth of the rivers, in planetary elevation units.
	mShader[mShaderCursor]->setUniform1f("RIVER_DEPTH", mNoiseParameter->riverDeapth);
    
    // Scaling to apply to the base continent elevations, in planetary elevation
    // units.
	mShader[mShaderCursor]->setUniform1f("CONTINENT_HEIGHT_SCALE", mNoiseParameter->continentHeightScale);
    
    // Calculate the sea level, in meters.
	mShader[mShaderCursor]->setUniform1f("SEA_LEVEL_IN_METRES", mNoiseParameter->seaLevelInMetres);
    //printf("seaLevelInMeters: %f\n", seaLevelInMeters);
    
    // Offset to apply to the terrain type definition.  Low values (< 1.0) cause
    // the rough areas to appear only at high elevations.  High values (> 2.0)
    // cause the rough areas to appear at any elevation.  The percentage of
    // rough areas on the planet are independent of this value.
    mShader[mShaderCursor]->setUniform1f("TERRAIN_OFFSET", mNoiseParameter->terrainOffset);
    
    // Specifies the "twistiness" of the mountains.
    mShader[mShaderCursor]->setUniform1f("MOUNTAINS_TWIST", mNoiseParameter->mountainsTwist);

    // Specifies the "twistiness" of the hills.
    mShader[mShaderCursor]->setUniform1f("HILLS_TWIST", mNoiseParameter->hillsTwist);

    // Specifies the "twistiness" of the badlands.
    mShader[mShaderCursor]->setUniform1f("BADLANDS_TWIST", mNoiseParameter->badlandsTwist);
    
    mShader[mShaderCursor]->setUniformMatrix("projection", mProjectionMatrix);
	mShader[mShaderCursor]->setUniformMatrix("texture", mRotation);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    mRenderGrid->render();    
    
    mShader[mShaderCursor]->unbind();
    if(3 == mShaderCursor || 4 == mShaderCursor)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE0);
    }
    return DR_OK;
}

// only for debugging
void RenderNoisePlanetToTexture::setFilenameToSave(DRString path)
{
    mFilename = path;
}

DRReturn RenderNoisePlanetToTexture::init(int stepSize, DRVector3 translate, float patchScaling, DRTexturePtr texture, const DRMatrix& rotation /*= DRMatrix::Identity()*/)
{
    mPatchScaling = patchScaling;
    mTranslate = translate;
    mRotation = rotation;
 //   maTextures[0] = DRTextureManager::Instance().getTexture(texture->getResolution(), 4);
    maTextures[0] = texture;
    return RenderInStepsToTexture::init(stepSize, maTextures[0]);    
}

DRReturn RenderNoisePlanetToTexture::reinit(DRTexturePtr texture)
{
    mShaderCursor = 0;
    //maTextures[0] = DRTextureManager::Instance().getTexture(texture->getResolution(), 4);
    maTextures[0] = texture;
    return RenderInStepsToTexture::reinit(maTextures[0]);
}

DRReturn RenderNoisePlanetToTexture::step()
{
    DRReturn ret = RenderInStepsToTexture::step();
    RenderToTexture::setFilenameToSave(mFilename);
 /*   if(mShaderCursor < 4 && isFinished())
    {
        mShaderCursor++;
        if(mShaderCursor < 4)
            maTextures[mShaderCursor] = DRTextureManager::Instance().getTexture(maTextures[mShaderCursor-1]->getResolution(), 4);

        RenderToTexture::setFilenameToSave(mFilename);
        
        if(RenderInStepsToTexture::reinit(maTextures[mShaderCursor]))
            LOG_ERROR("by reinit with step two", DR_ERROR);
    }
    */
    return ret;
}