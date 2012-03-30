#include "Sektor.h"

Sektor::Sektor(Vector3Unit position, Unit radius, SektorID id, Sektor* parent) 
: mID(id), mType(SEKTOR_NONE), mSektorPosition(position), mRadius(radius), mParent(parent), mThis(this),
  mRenderer(NULL), mIdleSeconds(0.0f), mNotRenderSeconds(0.0f)
{
    getSektorPath(mSektorPath);
    DRFileManager::addFolderToFileSystem(getSektorPathName().data());
}

Sektor::~Sektor()
{
    mChilds.clear();
    DR_SAVE_DELETE(mRenderer);
}

void Sektor::release()
{
	callForChilds(releaseChildCallback, NULL);
    //mParent.release();
    mThis.release();
}

void Sektor::setSektorSeed()
{
    u64 seed = mID;
    if(mParent)
        seed += mParent->getID();
    DRRandom::seed(static_cast<long>(seed));
    printf("[Sektor::setSektorSeed] randomSeed: %d\n", seed);
}
/*
DRReturn Sektor::move(float fTime, Camera* cam)
{
    DRReturn ret = DR_OK;
    for(std::map<u64, Sektor*>::iterator it = mChilds.begin(); it != mChilds.end(); it++)
    {
        Sektor* temp = it->second;
        ret = temp->move(fTime, cam);
        if(ret) LOG_ERROR("Fehler bei move sub-sektor", DR_ERROR);
    }
    
    return ret;
}

DRReturn Sektor::render(float fTime, Camera* cam)
{
    DRReturn ret = DR_OK;
    for(std::map<u64, Sektor*>::iterator it = mChilds.begin(); it != mChilds.end(); it++)
    {
        Sektor* temp = it->second;     
        ret = temp->render(fTime, cam);
        if(ret) LOG_ERROR("Fehler bei render sub-sektor", DR_ERROR);
    }
    
    return ret;
}

 */

DRReturn Sektor::renderAll(float fTime, Camera* cam, bool rootRendered/* = false*/)
{
    if(!rootRendered && mParent) return mParent->renderAll(fTime, cam, false);
    else if(!mParent || rootRendered) 
    {
        DRReturn ret = DR_OK;
        if(!mParent) ret = render(fTime, cam);
        if(ret) LOG_ERROR("Fehler bei render this", DR_ERROR);
        for(std::map<u64, SektorPtr>::iterator it = mChilds.begin(); it != mChilds.end(); it++)
        {
            SektorPtr temp = it->second;     
            ret = temp->render(fTime, cam);
            if(ret == DR_NOT_ERROR) 
            {
                ret = DR_OK;
                continue;
            }
            if(ret) LOG_ERROR("Fehler bei render", DR_ERROR);
            ret = temp->renderAll(fTime, cam, true);
            if(ret) LOG_ERROR("Fehler bei render all", DR_ERROR);
        }
        DRGrafikError("Sektor::renderAll");
        return ret;
    }
    return DR_ERROR;
}

DRReturn Sektor::moveAll(float fTime, Camera* cam, bool rootMoved/* = false*/)
{
    if(!rootMoved && mParent) return mParent->moveAll(fTime, cam, false);
    else if(!mParent || rootMoved) 
    {
        DRReturn ret = DR_OK;
        
		if(!mParent) ret = move(fTime, cam);
        for(std::map<u64, SektorPtr>::iterator it = mChilds.begin(); it != mChilds.end(); it++)
        {
            SektorPtr temp = it->second;     
            ret = temp->move(fTime, cam);
            if(ret) LOG_ERROR("Fehler bei move", DR_ERROR);
            
            temp->updateCameraSektor(cam);
            
            ret = temp->moveAll(fTime, cam, true);
            if(ret) LOG_ERROR("Fehler bei move all", DR_ERROR);
        }
        return ret;
    }
    return DR_ERROR;
}

void Sektor::updateCameraSektor(Camera* cam)
{
    if(!cam) return;
    Vector3Unit pos = cam->getSektorPositionAtSektor(mThis);
    //update camera position
    if(isObjectInSektor(pos))
    {
        if(cam->getCurrentSektor() != mThis && cam->getSektorPathSize() < mSektorPath.size())
        {
            cam->setSektorPosition(pos);
            cam->setCurrentSektor(mThis);            
            cam->updateSektorPath();            
            pos.convertTo(KM).print("new camera pos after switch lower");
        }
    }
    else
    {
        if(cam->getCurrentSektor() == mThis && mParent)
        {
            cam->setSektorPosition(cam->getSektorPositionAtSektor(mParent->getThis()));
            cam->setCurrentSektor(mParent->getThis());
            cam->updateSektorPath();     
            pos.convertTo(KM).print("new camera pos after switch higher");
        }
    }
}

SektorPtr Sektor::getSektorByPath(std::vector<SektorID>& path, int thisIndex /* = 0*/)
{
    if(thisIndex < 0) return SektorPtr();
    if(path.size() == 0) return SektorPtr();
    if(thisIndex && path[thisIndex] != mID) return SektorPtr();
    if(!thisIndex && *path.begin() != mID) return SektorPtr();
    if(path[path.size()-1] == mID) return mThis;
    
    SektorPtr child = getChild(path[++thisIndex]);
    if(!child.getResourcePtrHolder()->mResource)
    {
        printf("child: %uld\n", (u64)path[thisIndex]);
        LOG_ERROR("child didn't exist", SektorPtr());
    }
    //Sektor* child = mChilds.find(path[thisIndex])->second;// mChilds[path[thisIndex]];
    //if(!child) LOG_ERROR("child didn't exist", NULL);
    //return child->getSektorByPath(path, thisIndex+1);
	return child->getSektorByPath(path, thisIndex);
        
    return SektorPtr();
}

DRString Sektor::getSektorPathName() const
{
    std::stringstream s(std::stringstream::in|std::stringstream::out);

    std::vector<SektorID> path;
    getSektorPath(path);
    s << "./data/_" << path[0] << "/";
    for(uint i = 1; i < path.size(); i++)
    {
        s << "_" << path[i] << "/";
    }
    return s.str();
}

const char* Sektor::getSektorTypeName(SektorType type)
{
    switch(type)
    {
        case SEKTOR_ROOT:          return "root";
        case SUPER_GALAXIE_CLUSTER:     return "super_galaxie_cluster";
        case SUPER_GALAXIE:     return "super_galaxie";
        case GALAXIE_CLUSTER: return "galaxie_cluster";
        case GALAXIE:       return "galaxie";
        case STAR_CLUSTER:  return "star_cluster";
        case SOLAR_SYSTEM:  return "solar_system";
        case STELLAR_BODY:  return "stellar_body";
        case PLANET:        return "plante";
        case SUB_PLANET:    return "sub_planet";
        case SEKTOR_NONE:   return "none";
        default:            return "- default -";
    }
    return "- return -";
}

bool Sektor::isObjectInSektor(Vector3Unit positionInSektor)
{
    //return Vector3Unit(positionInSektor - mSektorPosition).lengthSq() <= mRadius*mRadius;
    return positionInSektor.lengthSq() <= mRadius*mRadius;
}

void Sektor::getSektorPath(std::vector<SektorID>& storage) const
{
    if(!mParent)
        storage.clear();
    else
        mParent->getSektorPath(storage);
    
    storage.push_back(mID);
}

void Sektor::removeInactiveChilds(double idleThreshold/* = 1.0*/)
{
    for(std::map<u64, SektorPtr>::iterator it = mChilds.begin(); it != mChilds.end(); it++)
    {
        SektorPtr temp = it->second;
        if(temp->mIdleSeconds < idleThreshold) continue;
        temp->release();
        mChilds.erase(it);
		break;
	}
}

DRReturn Sektor::callForChilds(DRReturn (*callbackFunction)(SektorPtr sektor, void* data), void* data)
{
    DRReturn ret = DR_OK;
    for(std::map<u64, SektorPtr>::iterator it = mChilds.begin(); it != mChilds.end(); it++)
    {
        SektorPtr temp = it->second;
        ret = callbackFunction(temp, data);  
        if(ret) LOG_ERROR("Fehler bei callbackFunction", DR_ERROR);
	}
    return ret;
}

DRReturn Sektor::releaseChildCallback(SektorPtr sektor, void* data)
{
    sektor->release();
    return DR_OK;
}