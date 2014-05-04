#include "PlanetSektor.h"
#include "ShaderManager.h"
#include "GlobalRenderer.h"
#include "noise/noise.h"
#include "SubPlanetSektor.h"
#include "Camera.h"


SektorID PlanetSektor::mSubPlanets[] = {SektorID(0,0,-1000, 0),SektorID(1000,0,0, 1),SektorID(0,0, 1000, 2),// front, right, back
                                        SektorID(-1000,0,0, 3),SektorID(0,1000,0, 4),SektorID(0,-1000,0, 5)};// left, top, bottom
                                            // left, up, right, down
int      PlanetSektor::mSubPlanetNeighborIndices[] = {5, 1, 4, 3, // 0, front
                                                      0, 5, 2, 4, // 1, right
                                                      1, 5, 3, 4, // 2, back
                                                      2, 5, 0, 4, // 3, right
                                                      1, 2, 3, 0, // 4, top
                                                      1, 0, 3, 2}; // 5, bottom     

PlanetSektor::PlanetSektor(Vector3Unit position, Unit radius, SektorID id, Sektor* parent)
: Sektor(position, radius, id, parent), mSphericalShaderForSubPlanet(NULL), mTheta(0.0f),
  mReadyCount(0)
{
    mType = PLANET;
    DRFileManager::addFolderToFileSystem(getSektorPathName().data());
    
    mSphericalShaderForSubPlanet = ShaderManager::Instance().getShaderProgram("sphere.vert", "sphere.frag");
        
    noise::module::Perlin p;
    if(id.count) p.SetSeed(id.count);
    DRVector3 idVector(id.x, id.y, id.z);
    idVector /= SHRT_MAX;
    setSektorSeed();
    
    const DRReal lacunarityMax = 2.4f;
    const DRReal lacunarityMin = 1.6f;
    
    // init values for planet perlin noise
    // Frequency of the planet's continents.  Higher frequency produces smaller,
    // more numerous continents.  This value is measured in radians.
	mPlanetNoiseParameters.continentFrequenzy = DRRandom::rReal(PI, PI/4.0f);
    
    // Lacunarity of the planet's continents.  Changing this value produces
    // slightly different continents.  For the best results, this value should
    // be random, but close to 2.0.
    mPlanetNoiseParameters.continentLacunarity = DRRandom::rReal(lacunarityMax, lacunarityMin);
    
    // Lacunarity of the planet's mountains.  Changing this value produces
    // slightly different mountains.  For the best results, this value should
    // be random, but close to 2.0.
    mPlanetNoiseParameters.mountainLacunarity = DRRandom::rReal(lacunarityMax, lacunarityMin);
    
    // Lacunarity of the planet's hills.  Changing this value produces slightly
    // different hills.  For the best results, this value should be random, but
    // close to 2.0.
    mPlanetNoiseParameters.hillsLacunarity = DRRandom::rReal(lacunarityMax, lacunarityMin);
    
    
    // Lacunarity of the planet's plains.  Changing this value produces slightly
    // different plains.  For the best results, this value should be random, but
    // close to 2.0.
    mPlanetNoiseParameters.plainsLacunarity = DRRandom::rReal(lacunarityMax, lacunarityMin);
    
    // Lacunarity of the planet's badlands.  Changing this value produces
    // slightly different badlands.  For the best results, this value should be
    // random, but close to 2.0.
    mPlanetNoiseParameters.badlandsLacunarity = DRRandom::rReal(lacunarityMax, lacunarityMin);
    
    
    // Specifies the amount of "glaciation" on the mountains.  This value
    // should be close to 1.0 and greater than 1.0. (1.375)
    mPlanetNoiseParameters.mountainGlaciation = DRRandom::rReal(1.6f, 1.01f);
 
    // Specifies the planet's sea level.  This value must be between -1.0
    // (minimum planet elevation) and +1.0 (maximum planet elevation.)
    const DRReal SEA_LEVEL = DRRandom::rReal(0.2f, -0.2f);
    mPlanetNoiseParameters.seaLevel = SEA_LEVEL;
    
    // Specifies the level on the planet in which continental shelves appear.
    // This value must be between -1.0 (minimum planet elevation) and +1.0
    // (maximum planet elevation), and must be less than SEA_LEVEL. (-0.375)
    mPlanetNoiseParameters.shelfLevel = DRRandom::rReal(SEA_LEVEL-0.01f, -0.8f);
    
    // Determines the amount of mountainous terrain that appears on the
    // planet.  Values range from 0.0 (no mountains) to 1.0 (all terrain is
    // covered in mountains).  Mountainous terrain will overlap hilly terrain.
    // Because the badlands terrain may overlap parts of the mountainous
    // terrain, setting MOUNTAINS_AMOUNT to 1.0 may not completely cover the
    // terrain in mountains.
    const DRReal MOUNTAINS_AMOUNT = DRRandom::rReal(0.9f, 0.1f);
    mPlanetNoiseParameters.mountainAmount = MOUNTAINS_AMOUNT;
  
    // Determines the amount of hilly terrain that appears on the planet.
    // Values range from 0.0 (no hills) to 1.0 (all terrain is covered in
    // hills).  This value must be less than MOUNTAINS_AMOUNT.  Because the
    // mountainous terrain will overlap parts of the hilly terrain, and
    // the badlands terrain may overlap parts of the hilly terrain, setting
    // HILLS_AMOUNT to 1.0 may not completely cover the terrain in hills.
    mPlanetNoiseParameters.hillsAmount = (1.0f + MOUNTAINS_AMOUNT) / 2.0f;
    
    // Determines the amount of badlands terrain that covers the planet.
    // Values range from 0.0 (no badlands) to 1.0 (all terrain is covered in
    // badlands.)  Badlands terrain will overlap any other type of terrain. (0.03125)
    mPlanetNoiseParameters.badlandsAmount = DRRandom::rReal(0.75f, 0.1f);
    
    // Maximum depth of the rivers, in planetary elevation units. (0.0234375f)
    mPlanetNoiseParameters.riverDeapth = DRRandom::rReal(0.02f, 0.0f);
    
    // Scaling to apply to the base continent elevations, in planetary elevation
    // units.
    mPlanetNoiseParameters.continentHeightScale = (1.0f - SEA_LEVEL) / 4.0f;
    
    // Calculate the sea level, in meters.
    DRReal MAX_ELEV = 1.0f; DRReal MIN_ELEV = -1.0f;
    DRReal seaLevelInMeters = (((SEA_LEVEL + 1.0f) / 2.0f)
    * (MAX_ELEV - MIN_ELEV)) + MIN_ELEV;
    mPlanetNoiseParameters.seaLevelInMetres = seaLevelInMeters;
    
    mPlanetNoiseParameters.maxHeightInPercent = DRRandom::rReal(0.005f, 0.001f);
    mPlanetNoiseParameters.minHeightInPercent = DRRandom::rReal(0.003f, 0.001f);
    
    // Offset to apply to the terrain type definition.  Low values (< 1.0) cause
    // the rough areas to appear only at high elevations.  High values (> 2.0)
    // cause the rough areas to appear at any elevation.  The percentage of
    // rough areas on the planet are independent of this value.
    mPlanetNoiseParameters.terrainOffset = DRRandom::rReal(0.8f, 2.4f);
    
    // Specifies the "twistiness" of the mountains.
    mPlanetNoiseParameters.mountainsTwist = DRRandom::rReal(0.75f, 1.25f);

    // Specifies the "twistiness" of the hills.
    mPlanetNoiseParameters.hillsTwist = DRRandom::rReal(0.75f, 1.25f);

    // Specifies the "twistiness" of the badlands.
    mPlanetNoiseParameters.badlandsTwist = DRRandom::rReal(0.75f, 1.25f);

    
    mPlanetNoiseParameters.print(true);
    
    mRenderer = new RenderPlanet(getSektorPathName(), &mPlanetNoiseParameters);
}

PlanetSektor::~PlanetSektor()
{
    //ShaderManager::Instance().releaseShader("sphere.vert", "sphere.frag");
    mSphericalShaderForSubPlanet = NULL;
    DR_SAVE_DELETE(mRenderer);
}

DRReturn PlanetSektor::move(float fTime, Camera* cam)
{
    
    //mLastRelativeCameraPosition = cam->getSektorPositionAtSektor(this);
    Sektor::move(fTime, cam);
    //if(mParent) mLastRelativeCameraPosition = mParent->getCameraPosition() - getPosition();
    //else mLastRelativeCameraPosition = cam->getSektorPositionAtSektor(this);
   // mTheta = acos(mRadius/mLastRelativeCameraPosition.length())*RADTOGRAD;
    
    std::vector<int>* ebene = GlobalRenderer::Instance().getEbenenCount();
	char buffer[256]; memset(buffer, 0, 256);
    
    for(uint i = 1; i < ebene->size(); i++)
        sprintf(buffer, "%s %d ", buffer, (*ebene)[i]);
    sprintf(buffer, "%s %d ", buffer, GlobalRenderer::Instance().getRenderTaskCount());
    //printf("\r[PlanetSektor::move]%s", buffer);
    if(EnIsButtonPressed(SDL_SCANCODE_K))
        cam->setAxis(DRVector3(-1.0f, 0.0f, 0.0f), DRVector3(0.0f, 1.0f, 0.0f), DRVector3(0.0f, 0.0f, -1.0f));
    
    RenderPlanet* renderer = static_cast<RenderPlanet*>(mRenderer);
    if(!renderer->isFinishLoading())
        renderer->generateTexture();
    
    //if(isObjectInSektor(mLastRelativeCameraPosition))
    if(mTheta <= 75.0f)
    {                
        for(u32 i = 0; i < 6; i++)
        {
            getChild(mSubPlanets[i]);            
            /*//horizont culling
            DRVector3 camPos = mLastRelativeCameraPosition.getVector3().normalize();
            double angle = acos(camPos.dot(DRVector3(mSubPlanets[i].x, mSubPlanets[i].y, mSubPlanets[i].z)))*RADTOGRAD-45.0;            
            //printf("\r %d, angle: %f (%f Grad) ", i, angle, angle*RADTOGRAD);
            if(angle < mTheta)
            {
                getChild(mSubPlanets[i]*static_cast<short>(1000));            
            }
            //*/
            //else
                //printf("\r %d, angle: %f, horizontAngle: %f", i, angle*RADTOGRAD, horizontAngle*RADTOGRAD);
        }
        //set neighbor pointers
        for(u32 iChild = 0; iChild < 6; iChild++)
        {
            if(mChilds.find(mSubPlanets[iChild]) == mChilds.end())
                    LOG_ERROR("sub planet missing", DR_ERROR);
            SubPlanetSektor* child = static_cast<SubPlanetSektor*>(mChilds[mSubPlanets[iChild]]);
            for(u8 i = 0; i < 4; i++)
            {
                child->setNeighbor(i, static_cast<SubPlanetSektor*>(getChild(mSubPlanets[mSubPlanetNeighborIndices[iChild*4+i]])));
            }
        }
    }
    else
    {
        //removeInactiveChilds(1.0f);
        removeInactiveChilds(GlobalRenderer::Instance().getTimeForInactiveChilds());
        if(mChilds.size() < 6) mReadyCount = 0;
    }
    
    
    return DR_OK;
}

DRReturn PlanetSektor::updateVisibility(const std::list<Camera*>& cameras)
{
    mTheta = 360.0f;
    RenderPlanet* render = dynamic_cast<RenderPlanet*>(mRenderer);
    DRReal distance = 100000.0f;
        
    for(std::list<Camera*>::const_iterator it = cameras.begin(); it != cameras.end(); it++)
    {
        double divisor = mRadius/(*it)->getSektorPositionAtSektor(this).length();
        double local_theta = 0.0f;
        if(divisor <= 1.0 && divisor >= -1.0) 
            local_theta = acos(divisor)*RADTOGRAD;
        
        if(local_theta < mTheta)
            mTheta = local_theta;
        Vector3Unit pos = (*it)->getSektorPositionAtSektor(this);
        DRReal local_distance = static_cast<DRReal>(pos.length().convertTo(M));
        if(local_distance < distance) distance = local_distance;
        
        if(it == cameras.begin())
        {
            Unit distance = pos.length()-mRadius;
            distance = distance.convertTo(KM);       
            //printf("\rdistance: %.3f KM, theta: %f", static_cast<double>(distance), mTheta);
        }
    }
       
    if(render && render->getRenderNoisePlanetToTexture())
    {
        if(mIdleSeconds > 0.0f) distance *= 1000.0f;
        render->getRenderNoisePlanetToTexture()->setCurrentDistance(0);
        // delete only, if childs have there own textures ready
        /*if(mNotRenderSeconds > GlobalRenderer::Instance().getTimeForInactiveChilds())
            DR_SAVE_DELETE(mRenderer);*/
    }
    
    
    return DR_OK;
}

DRReturn PlanetSektor::render(float fTime, Camera* cam)
{
#if SDL_VERSION_ATLEAST(1,3,0)
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
#else
	Uint8 *keystate = SDL_GetKeyState(NULL);
#endif
    RenderPlanet* renderer = static_cast<RenderPlanet*>(mRenderer);
    if(!renderer->isFinishLoading()) return DR_NOT_ERROR;
    
    //if(isObjectInSektor(cam->getSektorPosition())) return DR_OK;
	//Unit distance1 = Vector3Unit(mSektorPosition - cam->getSektorPosition()).length();
    //Unit distance1 = Vector3Unit(mSektorPosition - mLastRelativeCameraPosition).length();    
    Unit distance1 = (-mLastRelativeCameraPosition).length();    
	//DRVector3 diff = Vector3Unit(mSektorPosition - cam->getSektorPosition()).convertTo(KM).getVector3();

    distance1 = distance1.convertTo(mRadius.getType());
    double distance2 = 2000.0f;
    Unit radius1 = mRadius;
    double radius2 = ((radius1 * distance2) / distance1);

    
	//DRVector3 pos = (mSektorPosition - cam->getSektorPosition()).getVector3().normalize();
    //DRVector3 pos = (mSektorPosition - mLastRelativeCameraPosition).getVector3().normalize();
    DRVector3 pos = (-mLastRelativeCameraPosition).getVector3().normalize();
//    DRVector3 relCameraPos = -pos*distance1/mRadius;
     pos *= static_cast<DRReal>(distance2);
     //printf("\r%f %f %f, %.8f, %s ", pos.x, pos.y, pos.z, radius2, (distance1-mRadius).print().data());
     g_distanceToGround = (distance1-mRadius);
										   //*/
    //glTranslatef(pos.x, pos.y, pos.z);
    //glScaled(radius2, radius2, radius2);
	
    mMatrix = DRMatrix::scaling(DRVector3(static_cast<DRReal>(radius2))) * DRMatrix::translation(pos) * cam->getCameraMatrixRotation();
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//if(mRenderer && !isObjectInSektor(cam->getSektorPosition()))
    //DRReturn ret = mRenderer->render(fTime, cam);
    //if(!isObjectInSektor(mLastRelativeCameraPosition))
    //printf(" readyCount: %d",(int)mReadyCount);
    if(isSectorVisibleFromPosition(mLastRelativeCameraPosition)
       && (mTheta >= 70.0f || mReadyCount != 63))
    {
        mNotRenderSeconds = 0.0f;
        if(static_cast<RenderPlanet*>(mRenderer)->isErrorOccured())
            DR_SAVE_DELETE(mRenderer);
        if(!mRenderer)
            mRenderer = new RenderPlanet(getSektorPathName(), &mPlanetNoiseParameters);
        if(!mRenderer) LOG_ERROR("no renderer", DR_ERROR);
        
        if(radius2 > 160.0f) mRenderer->setCurrentDetail(10);
        else if(radius2 > 140.0f) mRenderer->setCurrentDetail(9);
        else if(radius2 > 120.0f) mRenderer->setCurrentDetail(8);
        else if(radius2 > 90.0f) mRenderer->setCurrentDetail(7);
        else if(radius2 > 70.0f) mRenderer->setCurrentDetail(6);
        else if(radius2 > 30.0f) mRenderer->setCurrentDetail(5);
        else if(radius2 > 25.0f) mRenderer->setCurrentDetail(4);
        else if(radius2 > 15.0f) mRenderer->setCurrentDetail(3);
        else if(radius2 > 5.0f) mRenderer->setCurrentDetail(2);
        else if(radius2 > 1.0f) mRenderer->setCurrentDetail(1);
        else mRenderer->setCurrentDetail(0);
    
      //GlobalRenderer::getSingleton().getPlanetShaderPtr()->bind();
        ShaderProgram* shader = mRenderer->getShaderProgram();
        if(!shader) LOG_ERROR("RenderPlanet hasn't valid shader", DR_ERROR);
        shader->bind();

        shader->setUniformMatrix("modelview", mMatrix);
        shader->setUniformMatrix("projection", GlobalRenderer::Instance().getProjectionMatrix().transpose());
        DRGrafikError("PlanetSektor::render");
        
        glDisable(GL_CULL_FACE);
        DRReturn ret = mRenderer->render(fTime, cam);
        glEnable(GL_CULL_FACE);
        shader->unbind();
//		GlobalRenderer::getSingleton().getPlanetShaderPtr()->unbind();
        if(ret) LOG_ERROR("Fehler bei call planet renderer", DR_ERROR);
        //child didn't need to render
        return DR_NOT_ERROR;
        //return DR_OK;
    } 
    else
    {
        mNotRenderSeconds += fTime;
    }
    
    return DR_OK;
}

Sektor* PlanetSektor::getChild(SektorID childID)
{        
    if(mChilds.find(childID) == mChilds.end())
    {
        // subPlanet seiten eines Würfels mit Kantenlänge 2 (intern)
        // Kantenpunktlänge = Wurzel(3), radius = 1, Quadratmittelpunktlänge = 1
        // Wurzel(3)       1
        //  ------    =  -----   => Wurzel(3) = 1/faktor => faktor = 1/Wurzel(3)
        //     1         faktor
        //double faktor = 1.0/sqrt(3.0);// sqrtf(3.0) = ca. 1.73205, faktor = ca. 0.577
        //double faktorH = sqrt(2.0/sqrt(3.0)-1.0/3.0);

        // Position des Quadratmittelpunktes
        //Vector3Unit position = Vector3Unit(childID.x, childID.y, childID.z, KM)*mRadius*faktor;

        //position.print("planet pos");

        printf("[PlanetSektor::getChild] radius: %s\n", mRadius.print().data());
        //SubPlanetSektor* temp = new SubPlanetSektor(position, radius, childID, this, this, mRadius/mLastRelativeCameraPosition.length());
        //0.617940f theta bei 6 patches
        //0.85-0.83 theta bei 6*4 patches
        SubPlanetSektor* temp = new SubPlanetSektor(mRadius, childID, this, this, 1.0f, 1);
        
        mChilds.insert(SEKTOR_ENTRY(childID, temp));
    }
    
    return mChilds[childID];
}

bool PlanetSektor::isObjectInSektor(SektorObject* sektorObject)
{       
    Unit l = sektorObject->getSektorPositionAtSektor(this).length();

    double divisor = mRadius/l;
    // leave with true, if distance is < radius
    if(divisor > 1.0) return true;    
    //double theta = acos(mRadius/l)*RADTOGRAD; // if theta < 0.35 Grad, using planes
    double theta = acos(divisor)*RADTOGRAD;
    //printf("\rtheta: %f, distance: %s", theta, (l-mRadius).convertTo(KM).print().data());
    return theta <=89.822667f;
    /*
    Unit radiusSquare = mRadius.convertTo(AE)*6.0;
    radiusSquare *= radiusSquare;
    //Unit distance = Vector3Unit(positionInParentSektor - mSektorPosition).length();
    //return Vector3Unit(positionInParentSektor.convertTo(AE) - mSektorPosition).lengthSq() <= radiusSquare;    
    return positionInSektor.convertTo(AE).lengthSq() <= radiusSquare;    
     * */
}
bool PlanetSektor::isSectorVisibleFromPosition(Vector3Unit positionInSektor)
{
    Unit l = positionInSektor.length();
    
    double divisor = mRadius/l;
    // leave with true, if distance is < radius
    if(divisor > 1.0) return true;    
    
    double theta = acos(divisor)*RADTOGRAD; // if theta < 0.35 Grad, using planes
    //printf("\rtheta: %f (%f Grad)", theta, theta*RADTOGRAD);
    return theta <=89.94f;
}

//*************************************************************************************************
// PlanetNoiseParameter
//*************************************************************************************************
void PlanetNoiseParameter::print(bool toLog /*= false*/)
{
    if(toLog)
    {
        DREngineLog.writeToLog("----- PlanetNoiseParameter Begin ----");
        for(int i = 0; i < 21; i++)
        {
            DREngineLog.writeToLog("%s: %.4f", getFieldName(static_cast<PlanetNoiseParameterNames>(i)), values[i]);
        }
        DREngineLog.writeToLog("----- PlanetNoiseParameter Ende ----");
    }
    else
    {
        printf("----- PlanetNoiseParameter Begin ----");
        for(int i = 0; i < 21; i++)
        {
            printf("%s: %.4f", getFieldName(static_cast<PlanetNoiseParameterNames>(i)), values[i]);
        }
        printf("----- PlanetNoiseParameter Ende ----");
    }
}

const char* PlanetNoiseParameter::getFieldName(PlanetNoiseParameterNames feldName)
{
    switch(feldName)
    {
        case CONTINENT_FREQUENCY: return "CONTINENT_FREQUENCY";
        case CONTINENT_LACUNARITY: return "CONTINENT_LACUNARITY";
        case MOUNTAIN_LACUNARITY: return "MOUNTAIN_LACUNARITY";
        case HILLS_LACUNARITY: return "HILLS_LACUNARITY";
        case PLAINS_LACUNARITY: return "PLAINS_LACUNARITY";
        case BADLANDS_LACUNARITY: return "BADLANDS_LACUNARITY";
        case MOUNTAIN_GLACIATION: return "MOUNTAIN_GLACIATION";
        case SEA_LEVEL: return "SEA_LEVEL";
        case SHELF_LEVEL: return "SHELF_LEVEL";
        case MOUNTAINS_AMOUNT: return "MOUNTAINS_AMOUNT";
        case HILLS_AMOUNT: return "HILLS_AMOUNT";
        case BADLANDS_AMOUNT: return "BADLANDS_AMOUNT";
        case RIVER_DEPTH: return "RIVER_DEPTH";
        case CONTINENT_HEIGHT_SCALE: return "CONTINENT_HEIGHT_SCALE";
        case SEA_LEVEL_IN_METRES: return "SEA_LEVEL_IN_METRES";
        case MAX_HEIGHT_IN_PERCENT: return "MAX_HEIGHT_IN_PERCENT";
        case MIN_HEIGHT_IN_PERCENT: return "MIN_HEIGHT_IN_PERCENT";
        case TERRAIN_OFFSET: return "TERRAIN_OFFSET";
        case MOUNTAINS_TWIST: return "MOUNTAINS_TWIST";
        case HILLS_TWIST: return "HILLS_TWIST";
        case BADLANDS_TWIST: return "BADLANDS_TWIST";
        default: return "-- invalid --";
    }
        
    return "-- error --";      
}