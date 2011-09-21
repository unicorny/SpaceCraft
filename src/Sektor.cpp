/* 
 * File:   Sektor.cpp
 * Author: dario
 * 
 * Created on 20. August 2011, 18:58
 */

#include "main.h"

Sektor::Sektor(SektorID id /*= 0*/): mID(id)
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
    return DR_OK;
}
