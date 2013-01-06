/* 
 * File:   Server.cpp
 * Author: dario
 * 
 * Created on 20. August 2011, 19:12
 */

#include "Server.h"

std::map<u64, Server*> Server::mServerList;

Server::Server(u64 serverID)
: mServerID(serverID), mRootSektor(NULL)
{
}

Server::Server(const Server& orig) 
: mServerID(orig.mServerID), mRootSektor(NULL)
{
}

Server::~Server() 
{
    DR_SAVE_DELETE(mRootSektor);
}

Server* Server::getServer(u64 ServerID)
{
    std::map<u64, Server*>::iterator it = mServerList.find(ServerID);
    if(it == mServerList.end())
    {
        Server* n = new Server(ServerID);
        if(n->loadFromFile())
        {
            DR_SAVE_DELETE(n);
            LOG_ERROR("couldn't load server from file", NULL);
        }
        mServerList.insert(SERVER_ENTRY(ServerID, n));
        return n;
    }
    else 
    {
        return it->second;
    }
    return NULL;
}

void Server::freeAllServer()
{
    for(std::map<u64, Server*>::iterator it = mServerList.begin(); it != mServerList.end(); it++)
    {
        Server* temp = it->second;
        if(temp->saveToFile()) LOG_WARNING("couldn't save server to file");
        DR_SAVE_DELETE(temp);
    }
}

u64 Server::createNewServer()
{
    u64 newID = DRRandom::r64();
         
   // std::stringstream s(std::stringstream::in|std::stringstream::out);
    //s << "./data/_" << newID;
    //if(DRFileManager::addFolderToFileSystem(s.str().data()))
      //  LOG_ERROR("neuer Server Ordner konnte nicht angelegt werden!", 0);
    
    Server* serv = getServer(newID);
    DRRandom::seed((int)newID);
    serv->mRootSektor = new RootSektor(newID);
    
    return newID;
}

DRReturn Server::saveToFile()
{
    std::stringstream s(std::ios_base::out|std::ios_base::in);
    s << "./data/_" << mServerID << "/server";
    DRFile* f = getFileDescriptor();
    if(!f) LOG_ERROR("f is zero", DR_ZERO_POINTER);
    f->open(s.str().data(), "wb");
    
    if(!f->isOpen()) LOG_ERROR("can't open server file to write", DR_ERROR);
        
    char identifier[] = "serv";
    f->write(identifier, sizeof(char), 4);
    f->close();
   
    freeFileDescriptor(f);
    if(mRootSektor)
    {
        //save
    }
    return DR_OK;
}

DRReturn Server::loadFromFile()
{
    std::stringstream s(std::stringstream::in|std::stringstream::out);
    s << "./data/_" << mServerID << "/server";
    DRFile* f = getFileDescriptor();
    if(!f) LOG_ERROR("f is zero", DR_ZERO_POINTER);
    f->open(s.str().data(), false, "rb");
    
    if(f->isOpen())
    {
        char identifier[5];
        f->read(identifier, sizeof(char), 4);
        identifier[4] = 0;
        if(strcmp(identifier, "serv") != 0)
        {
            LOG_WARNING("identifier isn't correct, server-file maybe corrupt?");
            DREngineLog.writeToLog(identifier);
        }
        else
        {
        }
        f->close();
    }
    freeFileDescriptor(f);
    DRRandom::seed((int)mServerID);
    mRootSektor = new RootSektor(mServerID);
    
    if(mRootSektor)
    {
        //load
    }
    
    return DR_OK;
}
/*
Sektor* Server::getHome()
{
    std::stringstream s(std::stringstream::in|std::stringstream::out);
    s << "./data/_" << mServerID << "/home";
    FILE* f = fopen(s.str().data(), "rb");
    if(!f)
    {
        LOG_WARNING("home not exist, create new one");
        Sektor* temp = new Sektor(0);
        Vector3Unit position(0, LIGHTYEAR);
        int seed = 7;
        srand(seed);
        Unit radius(DRRandom::rDouble(72000, 1000), KM);
        temp->setStellarBody(new Planet(radius, position, seed, temp));
        //mSektoren.insert(SEKTOR_ENTRY(0, temp));
	return temp;
    }
	return NULL;
	
    
}

DRReturn Server::saveHome(Sektor* home)
{
    return DR_OK;
}
 * */