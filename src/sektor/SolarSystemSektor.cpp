#include "SolarSystemSektor.h"
#include "noise/noise.h"
#include "Camera.h"

SolarSystemSektor::SolarSystemSektor(Vector3Unit position, Unit radius, SektorID id, Sektor* parent)
: Sektor(position, radius, id, parent)
{
    mType = SOLAR_SYSTEM;
    DRFileManager::addFolderToFileSystem(getSektorPathName().data());
    
    //neuen Planeten erstellen
    Vector3Unit planetPosition(DRRandom::rVector3(7000), KM);
    planetPosition = Unit(200000, KM)*planetPosition.normalize();
    //planetPosition = Unit(10, AE)*planetPosition.normalize();
    //Vector3Unit planetPosition(DRRandom::rVector3(10), AE);//VY Canis Majoris
    //planetPosition = planetPosition.normalize()*12;
    //planetPosition = planetPosition.normalize();
    //planetPosition = planetPosition * Unit(0.2f, AE);
    planetPosition.print("[SolarSystemSektor::SolarSystemSektor] Planeten position");
    
    noise::module::Perlin p;
    
    if(id.count) p.SetSeed(id.count);
    DRVector3 idVector(id.x, id.y, id.z);
	idVector /= SHRT_MAX;
    int seed = (int)(p.GetValue(idVector.x, idVector.y, idVector.z)*INT_MAX);
    //Unit planetRadius(DRRandom::rDouble(7000, 6000), KM);
    Unit planetRadius(6371, KM);
    //Unit planetRadius(9.3048028075, AE); //VY Canis Majoris
    
        
    DREngineLog.writeToLog("[SolarSystemSektor::SolarSystemSektor] Planeten Radius: %s", planetRadius.print().data());
    
    //mCurrentSektor->setStellarBody(new Planet(radius, position, seed, mCurrentSektor));    
    Sektor* temp = new PlanetSektor(planetPosition, planetRadius, seed, this);
    mChilds.insert(SEKTOR_ENTRY(seed, temp));
}

SolarSystemSektor::~SolarSystemSektor()
{
    
}

DRReturn SolarSystemSektor::move(float fTime, Camera* cam)
{
    //mLastRelativeCameraPosition = cam->getSektorPositionAtSektor(this);
    if(mParent) mLastRelativeCameraPosition = mParent->getCameraPosition() - getPosition();
    if(fTime == 0.0f)
    {
        Sektor* temp = mChilds.begin()->second;
        DREngineLog.writeToLog("new Game, set camera");
        cam->lookAt(temp->getPosition().getVector3());
        return DR_NOT_ERROR;
    }
    
    return DR_OK;
}

DRReturn SolarSystemSektor::render(float fTime, Camera* cam)
{
    //mMatrix = cam->getKameraMatrixRotation();
    return DR_OK;
}

DRReturn SolarSystemSektor::updateVisibility(const std::list<Camera*>& cameras)
{
    return DR_OK;
}