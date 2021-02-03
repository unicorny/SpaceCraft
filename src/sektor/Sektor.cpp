#include "Sektor.h"
#include "Camera.h"

Sektor::Sektor(Vector3Unit position, Unit radius, SektorID id, Sektor* parent) 
: mID(id), mType(SEKTOR_NONE), mSektorPosition(position), mRadius(radius), mParent(parent), mRenderer(NULL),
  mIdleSeconds(0.0f), mNotRenderSeconds(0.0f)
{
    createSektorPath();
}


Sektor::~Sektor()
{
    for(std::map<u64, Sektor*>::iterator it = mChilds.begin(); it != mChilds.end(); it++)
    {
        Sektor* temp = it->second;
        temp->setParent(NULL);
        DR_SAVE_DELETE(temp);
    }
    mChilds.clear();
    DR_SAVE_DELETE(mRenderer);
}

void Sektor::setSektorSeed()
{
    u64 seed = mID;
    if(mParent)
        seed += mParent->getID();
    DRRandom::seed(static_cast<long>(seed));
    printf("[Sektor::setSektorSeed] randomSeed: %d\n", seed);
}
DRReturn Sektor::move(float fTime, Camera* cam)
{
    if(mParent) mLastRelativeCameraPosition = mParent->getCameraPosition() - getPosition();
    else mLastRelativeCameraPosition = cam->getSektorPositionAtSektor(this);
    if(mParent)
    {
        //if(!isObjectInSektor(mLastRelativeCameraPosition))    
        if(!isSectorVisibleFromPosition(mLastRelativeCameraPosition))
            mIdleSeconds += fTime;
        else
            mIdleSeconds = 0.0f;
    }
    return DR_OK;
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
        for(std::map<u64, Sektor*>::iterator it = mChilds.begin(); it != mChilds.end(); it++)
        {
            Sektor* temp = it->second;     
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
        for(std::map<u64, Sektor*>::iterator it = mChilds.begin(); it != mChilds.end(); it++)
        {
            Sektor* temp = it->second;     
            ret = temp->move(fTime, cam);
            if(ret) {
				//DREngineLog.writeToLog("ret value: %d", ret);
				LOG_ERROR("Fehler bei move", DR_ERROR);
			}
            
            temp->updateCameraSektor(cam);
            
            ret = temp->moveAll(fTime, cam, true);
            if(ret) LOG_ERROR("Fehler bei move all", DR_ERROR);
        }
        return ret;
    }
    return DR_ERROR;
}

DRReturn Sektor::updateVisibilityAll(const std::list<Camera*>& cameras, bool rootUpdated/* = false*/)
{
    if(!rootUpdated && mParent) return mParent->updateVisibilityAll(cameras, false);
    else if(!mParent || rootUpdated) 
    {
        DRReturn ret = DR_OK;

        if(!mParent) ret = updateVisibility(cameras);
        for(std::map<u64, Sektor*>::iterator it = mChilds.begin(); it != mChilds.end(); it++)
        {
            Sektor* temp = it->second;     
            ret = temp->updateVisibility(cameras);
            if(ret) LOG_ERROR("Fehler bei updateVisibility", DR_ERROR);
            
            for(std::list<Camera*>::const_iterator it2 = cameras.begin(); it2 != cameras.end(); it2++)
                temp->updateCameraSektor(*it2);

            ret = temp->updateVisibilityAll(cameras, true);
            if(ret) LOG_ERROR("Fehler bei updateVisibility all", DR_ERROR);
        }
        return ret;
    }
    return DR_ERROR;
}

void Sektor::updateCameraSektor(Camera* cam)
{
    if(!cam) return;
    Vector3Unit pos = cam->getSektorPositionAtSektor(this);
    
    //update camera position
    if(isObjectInSektor(cam)) // down
    {
        if(cam->getCurrentSektor() != this && cam->getSektorPathSize() < mSektorPath.size())
        {
            cam->setSektorPosition(pos);
            cam->setCurrentSektor(this);            
            cam->updateSektorPath();  
            pos.convertTo(KM).print("[Sektor::updateCameraSektor] camPos after switch lower");
            printTypeInfos("[Sektor::updateCameraSektor]");
            mParent->printTypeInfos("[Sektor::updateCameraSektor] parent ");
        }
    }
    else // up
    {
        if(cam->getCurrentSektor() == this && mParent)
        {
            cam->setSektorPosition(cam->getSektorPositionAtSektor(mParent));
            cam->setCurrentSektor(mParent);
            cam->updateSektorPath();     
            pos.convertTo(KM).print("[Sektor::updateCameraSektor] camPos after switch higher");
            mParent->printTypeInfos("[Sektor::updateCameraSektor]");
			if(mParent->getParent())
				mParent->getParent()->printTypeInfos("[Sektor::updateCameraSektor] parent ");
        }
    }
}

void Sektor::printTypeInfos(const char* name)
{
    printf("%s %s\n", name, getSektorTypeName(mType));
}

Sektor* Sektor::getSektorByPath(std::vector<SektorID>& path, int thisIndex /* = 0*/)
{
    if(thisIndex < 0) LOG_ERROR("thisIndex < 0", NULL);
    if(path.size() == 0) LOG_ERROR("path.size() == 0", NULL);
    if(thisIndex && path[thisIndex] != mID) LOG_ERROR("thisINdex && path[thisIndex] != mID", NULL);
    if(!thisIndex && *path.begin() != mID) LOG_ERROR("!thisIndex && *path.begin() != mID", NULL);
    //std::vector<SektorID> sektorPath;
    //getSektorPath(sektorPath);
    if(path[path.size()-1] == mID && mSektorPath.size() == path.size()) return this;
    
    if(thisIndex+1 >= static_cast<int>(path.size())) LOG_ERROR("thisIndex+1 >= path.size()", NULL);
    Sektor* child = getChild(path[++thisIndex]);
    if(!child)
    {
        printf("child: %uld\n", (u64)path[thisIndex]);
        LOG_ERROR("child didn't exist", NULL);
    }
    //Sektor* child = mChilds.find(path[thisIndex])->second;// mChilds[path[thisIndex]];
    //if(!child) LOG_ERROR("child didn't exist", NULL);
    //return child->getSektorByPath(path, thisIndex+1);
	return child->getSektorByPath(path, thisIndex);
        
   // return NULL;
}

DRString Sektor::getSektorPathName()
{
    std::stringstream s(std::stringstream::in|std::stringstream::out);

    const std::vector<SektorID> path = getSektorPath();
    SektorID sektorID = path[0];
    s << "./data/_" << sektorID << "/";
    for(uint i = 1; i < path.size(); i++)
    {
        sektorID = path[i];
        s << "_" << sektorID << "/";
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
        case PLANET:        return "planet";
        case SUB_PLANET:    return "sub_planet";
        case SEKTOR_NONE:   return "none";
        default:            return "- default -";
    }
    return "- return -";
}

bool Sektor::isObjectInSektor(SektorObject* sektorObject)
{
    //return Vector3Unit(positionInSektor - mSektorPosition).lengthSq() <= mRadius*mRadius;
    //return positionInSektor.lengthSq() <= mRadius;
    return sektorObject->getSektorPositionAtSektor(this).lengthSq() <= mRadius;
}

bool Sektor::isSectorVisibleFromPosition(Vector3Unit positionInSektor)
{
    return positionInSektor.lengthSq() <= mRadius*mRadius;
}


void Sektor::createSektorPath()
{
    if(!mParent)
    {
        mSektorPath.clear();
    }
    else
    { 
        if(!mParent->getSektorPath().size())
            mParent->createSektorPath();
        mSektorPath = mParent->getSektorPathCopy();
    }
    
    mSektorPath.push_back(mID);
}

void Sektor::removeInactiveChilds(double idleThreshold/* = 1.0*/)
{
    for(std::map<u64, Sektor*>::iterator it = mChilds.begin(); it != mChilds.end(); it++)
    {
        Sektor* temp = it->second;
        if(temp->mIdleSeconds < idleThreshold) continue;
        if(temp->hasObserver()) continue;
        temp->setParent(NULL);
        DR_SAVE_DELETE(temp);
        mChilds.erase(it);
		break;
	}
}

DRReturn Sektor::callForChilds(DRReturn (*callbackFunction)(Sektor* sektor, void* data), void* data)
{
    DRReturn ret = DR_OK;
    for(std::map<u64, Sektor*>::iterator it = mChilds.begin(); it != mChilds.end(); it++)
    {
        Sektor* temp = it->second;
        ret = callbackFunction(temp, data);  
        if(ret) LOG_ERROR("Fehler bei callbackFunction", DR_ERROR);
	}
    return ret;
}

void Sektor::addObserver(DHASH hash, Observer* data)
{
    mObserver.insert(SEKTOR_OBSERVER(hash, data));
}

void Sektor::removeObserver(DHASH hash)
{
    if(mObserver.find(hash) != mObserver.end())
        mObserver.erase(hash);
}

bool Sektor::hasObserver()
{
    if(mObserver.empty())
    {
        for(std::map<u64, Sektor*>::iterator it = mChilds.begin(); it != mChilds.end(); it++)
        {
            if(it->second->hasObserver()) return true;
        }
        return false;
    }
    return true;
        
}