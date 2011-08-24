/* 
 * File:   Server.cpp
 * Author: dario
 * 
 * Created on 20. August 2011, 19:12
 */

#include "main.h"

Server::Server() {
}

Server::Server(const Server& orig) {
}

Server::~Server() {
}

Server* Server::getServer(u64 ServerID)
{
    return NULL;
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
