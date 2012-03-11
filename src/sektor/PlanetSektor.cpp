#include "PlanetSektor.h"
#include "ShaderManager.h"
#include "GlobalRenderer.h"
#include "noise/noise.h"
#include "SubPlanetSektor.h"


SektorID PlanetSektor::mSubPlanets[] = {SektorID(0,0,-1),SektorID(1,0,0),SektorID(0,0, 1),// front, right, back
                                        SektorID(-1,0,0),SektorID(0,1,0),SektorID(0,-1,0)};// left, top, bottom

PlanetSektor::PlanetSektor(Vector3Unit position, Unit radius, SektorID id, Sektor* parent)
: Sektor(position, radius, id, parent), mSphericalShaderForSubPlanet(NULL), mTheta(0.0f)
{
    mType = PLANET;
    
    mSphericalShaderForSubPlanet = ShaderManager::Instance().getShader("sphere.vert", "sphere.frag");
        
    noise::module::Perlin p;
    if(id.count) p.SetSeed(id.count);
    DRVector3 idVector(id.x, id.y, id.z);
    idVector /= SHRT_MAX;
    setSektorSeed();
    
    const double lacunarityMax = 2.4;
    const double lacunarityMin = 1.6;
    
    // init values for planet perlin noise
    // Frequency of the planet's continents.  Higher frequency produces smaller,
    // more numerous continents.  This value is measured in radians.
	mPlanetNoiseParameters.continentFrequenzy = DRRandom::rDouble(PI, PI/4.0);
    
    // Lacunarity of the planet's continents.  Changing this value produces
    // slightly different continents.  For the best results, this value should
    // be random, but close to 2.0.
    mPlanetNoiseParameters.continentLacunarity = DRRandom::rDouble(lacunarityMax, lacunarityMin);
    
    // Lacunarity of the planet's mountains.  Changing this value produces
    // slightly different mountains.  For the best results, this value should
    // be random, but close to 2.0.
    mPlanetNoiseParameters.mountainLacunarity = DRRandom::rDouble(lacunarityMax, lacunarityMin);
    
    // Lacunarity of the planet's hills.  Changing this value produces slightly
    // different hills.  For the best results, this value should be random, but
    // close to 2.0.
    mPlanetNoiseParameters.hillsLacunarity = DRRandom::rDouble(lacunarityMax, lacunarityMin);
    
    
    // Lacunarity of the planet's plains.  Changing this value produces slightly
    // different plains.  For the best results, this value should be random, but
    // close to 2.0.
    mPlanetNoiseParameters.plainsLacunarity = DRRandom::rDouble(lacunarityMax, lacunarityMin);
    
    // Lacunarity of the planet's badlands.  Changing this value produces
    // slightly different badlands.  For the best results, this value should be
    // random, but close to 2.0.
    mPlanetNoiseParameters.badlandsLacunarity = DRRandom::rDouble(lacunarityMax, lacunarityMin);
    
    
    // Specifies the amount of "glaciation" on the mountains.  This value
    // should be close to 1.0 and greater than 1.0. (1.375)
    mPlanetNoiseParameters.mountainGlaciation = DRRandom::rDouble(1.6, 1.01);
 
    // Specifies the planet's sea level.  This value must be between -1.0
    // (minimum planet elevation) and +1.0 (maximum planet elevation.)
    const float SEA_LEVEL = DRRandom::rDouble(0.1, -0.8);
    mPlanetNoiseParameters.seaLevel = SEA_LEVEL;
    
    // Specifies the level on the planet in which continental shelves appear.
    // This value must be between -1.0 (minimum planet elevation) and +1.0
    // (maximum planet elevation), and must be less than SEA_LEVEL. (-0.375)
    mPlanetNoiseParameters.shelfLevel = DRRandom::rDouble(SEA_LEVEL-0.01, -0.8);
    
    // Determines the amount of mountainous terrain that appears on the
    // planet.  Values range from 0.0 (no mountains) to 1.0 (all terrain is
    // covered in mountains).  Mountainous terrain will overlap hilly terrain.
    // Because the badlands terrain may overlap parts of the mountainous
    // terrain, setting MOUNTAINS_AMOUNT to 1.0 may not completely cover the
    // terrain in mountains.
    const float MOUNTAINS_AMOUNT = DRRandom::rDouble(0.9, 0.1);
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
    mPlanetNoiseParameters.badlandsAmount = DRRandom::rDouble(0.75, 0.1);
    
    // Maximum depth of the rivers, in planetary elevation units. (0.0234375f)
    mPlanetNoiseParameters.riverDeapth = DRRandom::rDouble(0.02, 0.0);
    
    // Scaling to apply to the base continent elevations, in planetary elevation
    // units.
    mPlanetNoiseParameters.continentHeightScale = (1.0f - SEA_LEVEL) / 4.0f;
    
    // Calculate the sea level, in meters.
    float MAX_ELEV = 1.0f; float MIN_ELEV = -1.0f;
    float seaLevelInMeters = (((SEA_LEVEL + 1.0f) / 2.0f)
    * (MAX_ELEV - MIN_ELEV)) + MIN_ELEV;
    mPlanetNoiseParameters.seaLevelInMetres = seaLevelInMeters;
    
    mPlanetNoiseParameters.maxHeightInPercent = DRRandom::rDouble(0.005, 0.0005);
    mPlanetNoiseParameters.minHeightInPercent = DRRandom::rDouble(0.003, 0.001);
    mPlanetNoiseParameters.print(true);
    
    mRenderer = new RenderPlanet(id, getSektorPathName(), &mPlanetNoiseParameters);
}

PlanetSektor::~PlanetSektor()
{
    ShaderManager::Instance().releaseShader("sphere.vert", "sphere.frag");
    mSphericalShaderForSubPlanet = NULL;
    DR_SAVE_DELETE(mRenderer);
}

DRReturn PlanetSektor::move(float fTime, Camera* cam)
{
    mLastRelativeCameraPosition = cam->getSektorPositionAtSektor(this);
    mTheta = acos(mRadius/mLastRelativeCameraPosition.length());
    Unit distance = mLastRelativeCameraPosition.length()-mRadius;
    distance = distance.convertTo(KM);
    //printf("\rEntfernung zur Oberflaeche: %s", distance.print().data());
    //Vector3Unit cameraPlanet = -mLastRelativeCameraPosition;
    //Unit l = cameraPlanet.length();
    //double theta = acos(mRadius/l); // if theta < 0.5 Grad, using ebene
    //theta = cos(0.617940);
    //printf("\rtheta: %f (%f Grad), distance: %f", theta, theta*RADTOGRAD,  (float)mRadius/mLastRelativeCameraPosition.length());
//    mLastRelativeCameraPosition.print("cameraPos");
    DRVector3 v = mLastRelativeCameraPosition.getVector3();
    
    
    printf("\rdistance: %f", static_cast<double>(distance));
    
    if(isObjectInSektor(mLastRelativeCameraPosition))
    {                
        for(u32 i = 0; i < 6; i++)
        {
            //horizont culling
            DRVector3 camPos = mLastRelativeCameraPosition.getVector3().normalize();
            double angle = acos(camPos.dot(DRVector3(mSubPlanets[i].x, mSubPlanets[i].y, mSubPlanets[i].z)))-PI/4.0;            
            if(angle < mTheta)
            {
                getChild(mSubPlanets[i]*static_cast<short>(1000));            
            }
            //else
                //printf("\r %d, angle: %f, horizontAngle: %f", i, angle*RADTOGRAD, horizontAngle*RADTOGRAD);
        }
    }
    else
    {
        //removeInactiveChilds(1.0f);
    }
    removeInactiveChilds(GlobalRenderer::Instance().getTimeForInactiveChilds());
    return DR_OK;
}

DRReturn PlanetSektor::render(float fTime, Camera* cam)
{
#if SDL_VERSION_ATLEAST(1,3,0)
	Uint8 *keystate = SDL_GetKeyboardState(NULL);
#else
	Uint8 *keystate = SDL_GetKeyState(NULL);
#endif
    
    //if(isObjectInSektor(cam->getSektorPosition())) return DR_OK;
	//Unit distance1 = Vector3Unit(mSektorPosition - cam->getSektorPosition()).length();
    //Unit distance1 = Vector3Unit(mSektorPosition - mLastRelativeCameraPosition).length();    
    Unit distance1 = (-mLastRelativeCameraPosition).length();    
	//DRVector3 diff = Vector3Unit(mSektorPosition - cam->getSektorPosition()).convertTo(KM).getVector3();

    distance1 = distance1.convertTo(mRadius.getType());
    double distance2 = 200.0f;
    Unit radius1 = mRadius;
    double radius2 = ((radius1 * distance2) / distance1);

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

	//DRVector3 pos = (mSektorPosition - cam->getSektorPosition()).getVector3().normalize();
    //DRVector3 pos = (mSektorPosition - mLastRelativeCameraPosition).getVector3().normalize();
    DRVector3 pos = (-mLastRelativeCameraPosition).getVector3().normalize();
    DRVector3 relCameraPos = -pos*distance1/mRadius;
    pos *= distance2;
/*   printf("\r %f %f %f, %.8f, %s  x:%s y:%s z:%s (%f %f %f)", pos.x, pos.y, pos.z, radius2, distance1.print().data(),
									   absCameraPosition.x.print().data(), absCameraPosition.y.print().data(),
									   absCameraPosition.z.print().data(), diff.x, diff.y, diff.z);
										   //*/
    //glTranslatef(pos.x, pos.y, pos.z);
    //glScaled(radius2, radius2, radius2);
	
    mMatrix = DRMatrix::scaling(DRVector3(radius2)) * DRMatrix::translation(pos) * mParent->getMatrix() ;
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//if(mRenderer && !isObjectInSektor(cam->getSektorPosition()))
    //DRReturn ret = mRenderer->render(fTime, cam);
    if(mRenderer && !isObjectInSektor(mLastRelativeCameraPosition))
    {
      //GlobalRenderer::getSingleton().getPlanetShaderPtr()->bind();
        ShaderProgram* shader = mRenderer->getShaderProgram();
        if(!shader) LOG_ERROR("RenderPlanet hasn't valid shader", DR_ERROR);
        shader->bind();

        shader->setUniformMatrix("modelview", mMatrix);
        shader->setUniformMatrix("projection", GlobalRenderer::Instance().getProjectionMatrix().transpose());
        DRGrafikError("PlanetSektor::render");
        
        DRReturn ret = mRenderer->render(fTime, cam);
        shader->unbind();
//		GlobalRenderer::getSingleton().getPlanetShaderPtr()->unbind();
        if(ret) LOG_ERROR("Fehler bei call planet renderer", DR_ERROR);
        //child didn't need to render
        return DR_NOT_ERROR;
        //return DR_OK;
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
    //    double faktor = 1.0/sqrt(3.0);// sqrtf(3.0) = ca. 1.73205, faktor = ca. 0.577
     //   double faktorH = sqrt(2.0/sqrt(3.0)-1.0/3.0);

        // Position des Quadratmittelpunktes
        //Vector3Unit position = Vector3Unit(childID.x, childID.y, childID.z, KM)*mRadius*faktor;
        DRVector3 childPos(childID.x, childID.y, childID.z);
        childPos /= 1000.0f;
        childPos = childPos.normalize();
        printf("[PlanetSektor::getChild] pos: %f, %f, %f\n", childPos.x, childPos.y, childPos.z);
        Vector3Unit position = Vector3Unit(childPos.x, childPos.y, childPos.z, KM)*mRadius;//*faktor;
        //position.print("planet pos");

        printf("[PlanetSektor::getChild] radius: %s\n", mRadius.print().data());
        //SubPlanetSektor* temp = new SubPlanetSektor(position, radius, childID, this, this, mRadius/mLastRelativeCameraPosition.length());
        //0.617940f theta bei 6 patches
        //0.85-0.83 theta bei 6*4 patches
        SubPlanetSektor* temp = new SubPlanetSektor(position, mRadius, childID, this, this, 1.0f);
        
        mChilds.insert(SEKTOR_ENTRY(childID, temp));

        //Set neighbor pointer
        for(int i = 0; i < 6; i++)
        {
            SubPlanetSektor* n = NULL;
            if(i >= 1 && i <= 3) // right, back, left
            {
                if(mChilds.find(mSubPlanets[i-1]) != mChilds.end())
                {
                    n = dynamic_cast<SubPlanetSektor*>(mChilds[mSubPlanets[i-1]]);
                    if(temp == n) continue;
                    temp->setNeighbor(NEIGHBOR_LEFT, n);//left                    
                    n->setNeighbor(NEIGHBOR_RIGHT, temp);//right 
                }
            }
            else if(4 == i || 5 == i)//top, bottom
            {
                for(u32 j = 0; j < 4; j++)
                {
                    if(mChilds.find(mSubPlanets[j]) != mChilds.end())
                    {
                        n = dynamic_cast<SubPlanetSektor*>(mChilds[mSubPlanets[j]]);
                        if(temp == n) continue;
                        if(4 == i)
                        {
                            temp->setNeighbor(3-j, n);                    
                            n->setNeighbor(NEIGHBOR_UP, temp);
                        }
                        else
                        {
                            temp->setNeighbor(j, n);                    
                            n->setNeighbor(NEIGHBOR_DOWN, temp); 
                        }
                    }
                }
            }
        }
    }
    
    return mChilds[childID];
}

bool PlanetSektor::isObjectInSektor(Vector3Unit positionInSektor)
{    
    Unit l = positionInSektor.length();

    double theta = acos(mRadius/l); // if theta < 0.5 Grad, using ebene
    //printf("\rtheta: %f (%f Grad)", theta, theta*RADTOGRAD);
    return theta <=70.0f*GRADTORAD;
    /*
    Unit radiusSquare = mRadius.convertTo(AE)*6.0;
    radiusSquare *= radiusSquare;
    //Unit distance = Vector3Unit(positionInParentSektor - mSektorPosition).length();
    //return Vector3Unit(positionInParentSektor.convertTo(AE) - mSektorPosition).lengthSq() <= radiusSquare;    
    return positionInSektor.convertTo(AE).lengthSq() <= radiusSquare;    
     * */
}

//*************************************************************************************************
// PlanetNoiseParameter
//*************************************************************************************************
void PlanetNoiseParameter::print(bool toLog /*= false*/)
{
    if(toLog)
    {
        DRLog.writeToLog("----- PlanetNoiseParameter Begin ----");
        for(int i = 0; i < 17; i++)
        {
            DRLog.writeToLog("%s: %.4f", getFieldName(static_cast<PlanetNoiseParameterNames>(i)), values[i]);
        }
        DRLog.writeToLog("----- PlanetNoiseParameter Ende ----");
    }
    else
    {
        printf("----- PlanetNoiseParameter Begin ----");
        for(int i = 0; i < 17; i++)
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
        default: return "-- invalid --";
    }
    
    return "-- error --";      
}