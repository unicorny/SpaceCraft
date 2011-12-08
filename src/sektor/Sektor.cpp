#include "main.h"

Sektor::Sektor(Vector3Unit position, Unit radius, SektorID id, Sektor* parent) 
: mID(id), mType(SEKTOR_NONE), mSektorPosition(position), mRadius(radius), mParent(parent), mRenderer(NULL),
    mIdleSeconds(0.0)
{
    getSektorPath(mSektorPath);
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
			glPushMatrix();
            ret = temp->render(fTime, cam);
            if(ret == DR_NOT_ERROR) 
            {
                glPopMatrix();
				ret = DR_OK;
                continue;
            }
            if(ret) LOG_ERROR("Fehler bei render", DR_ERROR);
            ret = temp->renderAll(fTime, cam, true);
            if(ret) LOG_ERROR("Fehler bei render all", DR_ERROR);
			glPopMatrix();
        }
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
    Vector3Unit pos = cam->getSektorPositionAtSektor(this);
    //update camera position
    if(isObjectInSektor(pos))
    {
        if(cam->getCurrentSektor() != this && cam->getSektorPathSize() < mSektorPath.size())
        {
            cam->setSektorPosition(pos);
            cam->setCurrentSektor(this);            
            cam->updateSektorPath();            
            pos.convertTo(KM).print("new camera pos after switch lower");
        }
    }
    else
    {
        if(cam->getCurrentSektor() == this && mParent)
        {
            cam->setSektorPosition(cam->getSektorPositionAtSektor(mParent));
            cam->setCurrentSektor(mParent);
            cam->updateSektorPath();            
        }
    }
}

Sektor* Sektor::getSektorByPath(std::vector<SektorID>& path, int thisIndex)
{
    if(thisIndex < 0) return NULL;
	if(path.size() == 0) return NULL;
    if(thisIndex && path[thisIndex] != mID) return NULL;
    if(!thisIndex && *path.begin() != mID) return NULL;
    if(path[path.size()-1] == mID) return this;
    
    Sektor* child = getChild(path[++thisIndex]);
	if(!child) LOG_ERROR("child din't exist", NULL);
    //Sektor* child = mChilds.find(path[thisIndex])->second;// mChilds[path[thisIndex]];
    //if(!child) LOG_ERROR("child didn't exist", NULL);
    //return child->getSektorByPath(path, thisIndex+1);
	return child->getSektorByPath(path, thisIndex);
        
    return NULL;
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
    for(std::map<u64, Sektor*>::iterator it = mChilds.begin(); it != mChilds.end(); it++)
    {
        Sektor* temp = it->second;
        if(temp->mIdleSeconds < idleThreshold) continue;
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