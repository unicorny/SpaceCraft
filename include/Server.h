/* 
 * File:   Server.h
 * Author: dario
 *
 * Created on 20. August 2011, 19:12
 */

#ifndef __SC_SERVER_H
#define	__SC_SERVER_H

class Server {
public:    
    static Server* getServer(u64 ServerID);
    static void freeServer(Server* server) {DR_SAVE_DELETE(server);};
    static void freeServer(u64 ServerID) {LOG_WARNING("not implemented");}
    
    static u64 createNewServer();
    
    // home ist ein sonnensystem sektor
    Sektor* getHome();    
    DRReturn saveHome(Sektor* home);
    
private:
    Server(u64 serverID);
    Server(const Server& orig);
    
    virtual ~Server();
    
    u64 mServerID;
    std::map<u64, Sektor*> mSektoren;
    typedef std::pair<u64, Sektor*> SEKTOR_ENTRY;
};

#endif	/* SERVER_H */

