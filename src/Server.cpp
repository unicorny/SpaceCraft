/* 
 * File:   Server.cpp
 * Author: dario
 * 
 * Created on 20. August 2011, 19:12
 */

#include "main.h"

Server::Server(u64 serverID)
: mServerID(serverID)
{
}

Server::Server(const Server& orig) {
}

Server::~Server() 
{
    for(std::map<u64, Sektor*>::iterator it = mSektoren.begin(); it != mSektoren.end(); it++)
    {
        Sektor* temp = it->second;
        DR_SAVE_DELETE(temp);
    }
    mSektoren.clear();
}

Server* Server::getServer(u64 ServerID)
{
    return new Server(ServerID);
    //return NULL;
}

u64 Server::createNewServer()
{
    u64 newID = DRRandom::r64();
         
    std::stringstream s(std::stringstream::in|std::stringstream::out);
    s << "./data/_" << newID;
    if(DRFileManager::addFolderToFileSystem(s.str().data()))
        LOG_ERROR("neuer Server Ordner konnte nicht angelegt werden!", 0);
    
    return newID;
}

Sektor* Server::getHome()
{
    std::stringstream s(std::stringstream::in|std::stringstream::out);
    s << "./data/_" << mServerID << "/home";
    FILE* f = fopen(s.str().data(), "rb");
    if(!f)
    {
        LOG_WARNING("home not exist, create new one");
        Sektor* temp = new Sektor(0);
        Unit position[3];
        for(int i = 0; i < 3; i++)
            position[i] = Unit(0, LIGHTYEAR);
        int seed = 7;
        Unit radius(DRRandom::rDouble(72000, 1000), KM);
        temp->addStellarBody(new Planet(radius, position, seed, temp));
        mSektoren.insert(SEKTOR_ENTRY(0, temp));
    }
    
}

DRReturn Server::saveHome(Sektor* home)
{
    
}