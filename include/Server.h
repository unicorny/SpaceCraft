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
    static void freeServer(Server* server) {};
    static void freeServer(u64 ServerID) {};
    
    static u64 createNewServer();
    
    
private:
    Server();
    Server(const Server& orig);
    
    virtual ~Server();

};

#endif	/* SERVER_H */

