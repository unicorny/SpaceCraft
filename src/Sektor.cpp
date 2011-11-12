/* 
 * File:   Sektor.cpp
 * Author: dario
 * 
 * Created on 20. August 2011, 18:58
 */

#include "main.h"

Sektor::Sektor(Sektor* parent, SektorType type, SektorID id /*= 0*/, int seed /* = 0*/, u64 serverID/* = 0*/): 
mStellarBody(NULL), mID(id), mType(type), mSeed(seed), mParent(parent), mServerID(serverID)
{
}

Sektor::Sektor(const Sektor& orig)
: mStellarBody(NULL), mID(orig.mID), mType(orig.mType), mSeed(orig.mSeed), mParent(orig.mParent),
  mSektorSize(orig.mSektorSize), mSektorPosition(orig.mSektorPosition), mServerID(orig.mServerID)
{
}

Sektor::~Sektor() 
{
    DR_SAVE_DELETE(mStellarBody);
    
    for(std::map<u64, Sektor*>::iterator it = mChilds.begin(); it != mChilds.end(); it++)
    {
        Sektor* temp = it->second;
        DR_SAVE_DELETE(temp);
    }
    mChilds.clear();
}

void Sektor::setStellarBody(StellarBody* newBody)
{
    if(!newBody) LOG_ERROR_VOID("Zero Pointer");
    DR_SAVE_DELETE(mStellarBody);
    mStellarBody = newBody;
}

DRReturn Sektor::addSektor(Sektor* newSektor)
{
    SektorID s = newSektor->getID();
    s.count = 0;
    std::map<u64, Sektor*>::iterator it;
    do
    {
        s.count++;
        it = mChilds.find(s);
    } while(it != mChilds.end());
    if(s.count > 0) DRLog.writeToLog("[Sektor::addSektor] count value is: %d", s.count);
    newSektor->setID(s);
    
    mChilds.insert(SEKTOR_ENTRY(s, newSektor));
    
    return DR_OK;
}

DRReturn Sektor::save(DRFile* openFile)
{
    if(!openFile->isOpen()) LOG_ERROR("file isn't open, error by save sektor", DR_ERROR);
    LOG_WARNING("function isn't finished yet");
    char identifier[] = "sekt";
    openFile->write(identifier, sizeof(char), 4);
    
    return DR_OK;
}

DRReturn Sektor::render(float fTime, Camera* camera)
{        
    //Reseten der Matrixen
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, (GLfloat)XWIDTH/(GLfloat)YHEIGHT, 0.001f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);          // Select the modelview matrix

    glLoadIdentity();                    // Reset (init) the modelview matrix
    camera->setKameraMatrixRotation();
    
    //glDisable(GL_DEPTH_TEST);             // Enables depth test
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    if(!mStellarBody) return DR_OK;
    
    //glScalef(10.0f, 10.0f, 10.0f);
    //glTranslatef(0.0f, 0.0f, -200.0f);
    mStellarBody->render(fTime, camera->getAbsPosition());
    
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    return DR_OK;
}

DRReturn Sektor::move(float fTime, Camera* camera)
{
    if(!camera) LOG_ERROR("Zero Pointer: camera", DR_ZERO_POINTER);
    
    Vector3Unit diff(camera->getAbsPosition()-mLastPosition);
    if(diff.lengthSq())
    {
        //diff.print();
              
    }   
    
    mLastPosition = camera->getAbsPosition();
    return DR_OK;
}

std::list<u64>* Sektor::getSektorPath(std::list<u64>* buffer)
{
    if(!buffer) return NULL;
    if(mType == ROOT) return buffer;
    if(mParent)
        mParent->getSektorPath(buffer);
    buffer->push_back(mID.id);
    return buffer;
}

const char* Sektor::getSektorTypeName(SektorType type)
{
    switch(type)
    {
        case ROOT: return "root";
        case SUPER_GALAXIE_CLUSTER: return "super_galaxie_cluster";
        case SUPER_GALAXIE: return "super_galaxie";
        case GALAXIE_CLUSTER: return "galaxie_cluster";
        case GALAXIE: return "galaxie";
        case STAR_CLUSTER: return "star_cluster";
        case SOLAR_SYSTEM: return "solar_system";
        case STELLAR_BODY: return "stellar_body";
        case SEKTOR_NONE: return "none";
        default: return "- default -";
    }
    return "- return -";
}