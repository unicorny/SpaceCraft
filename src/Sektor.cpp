/* 
 * File:   Sektor.cpp
 * Author: dario
 * 
 * Created on 20. August 2011, 18:58
 */

#include "main.h"

Sektor::Sektor(SektorID id /*= 0*/, int seed /* = 0*/): mID(id), mSeed(seed)
{
}

Sektor::Sektor(const Sektor& orig)
: mID(orig.mID)
{
}

Sektor::~Sektor() 
{
    for(std::list<StellarBody*>::iterator it = mStellarBodys.begin(); it != mStellarBodys.end(); it++)
    {
        DR_SAVE_DELETE(*it);
    }
    mStellarBodys.clear();
}

void Sektor::addStellarBody(StellarBody* newBody)
{
    if(!newBody) LOG_ERROR_VOID("Zero Pointer");
    mStellarBodys.push_back(newBody);
}

DRReturn Sektor::save(DRFile* openFile)
{
    if(!openFile->isOpen()) LOG_ERROR("file isn't open, error by save sektor", DR_ERROR);
    LOG_WARNING("function isn't finished yet");
    char identifier[] = "sekt";
    openFile->write(identifier, sizeof(char), 4);
    
    for(std::list<StellarBody*>::iterator it = mStellarBodys.begin(); it != mStellarBodys.end(); it++)
    {
     
    }
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
    
    
    //glScalef(10.0f, 10.0f, 10.0f);
    //glTranslatef(0.0f, 0.0f, -200.0f);
    
    for(std::list<StellarBody*>::iterator it = mStellarBodys.begin(); it != mStellarBodys.end(); it++)
    {
        (*it)->render(fTime, camera->getAbsPosition());
    }    
    
    
    return DR_OK;
}

DRReturn Sektor::move(float fTime, Camera* camera)
{
    if(!camera) LOG_ERROR("Zero Pointer: camera", DR_ZERO_POINTER);
    
    Vector3Unit diff(camera->getAbsPosition()-mLastPosition);
    if(diff.lengthSq())
    {
        //diff.print();
        for(std::list<StellarBody*>::iterator it = mStellarBodys.begin(); it != mStellarBodys.end(); it++)
        {
            
        }        
    }   
    
    mLastPosition = camera->getAbsPosition();
    return DR_OK;
}