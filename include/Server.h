/* 
 * File:   Server.h
 * Author: dario
 *
 * Created on 20. August 2011, 19:12
 */

#ifndef __SC_SERVER_H
#define	__SC_SERVER_H

#include "RootSektor.h"

class Server {
public:    
    static Server* getServer(u64 ServerID);
    static void freeServer(Server* server) {DR_SAVE_DELETE(server);};
    static void freeServer(u64 ServerID) {LOG_WARNING("not implemented");}
    static void freeAllServer();
    
    static u64 createNewServer();
        
    // later, DRFile will be overloaded to enable write and read on a far server
    DRFile* getFileDescriptor() {return new DRFile();}
    void freeFileDescriptor(DRFile* fileDescriptor) {fileDescriptor->close(); DR_SAVE_DELETE(fileDescriptor);}
    
    DRReturn saveToFile();
    DRReturn loadFromFile();
    
    RootSektor* getRootSektor() {return mRootSektor;}
    
private:
    Server(u64 serverID);
    Server(const Server& orig);
    static std::map<u64, Server*> mServerList;
    typedef std::pair<u64, Server*> SERVER_ENTRY;
    
    virtual ~Server();
    
    u64 mServerID;
    RootSektor* mRootSektor;
};

#endif	/* SERVER_H */

