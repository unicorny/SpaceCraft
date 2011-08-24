/* 
 * File:   Player.h
 * Author: dario
 *
 * Created on 20. August 2011, 18:56
 */

#ifndef __SC_PLAYER_H
#define	__SC_PLAYER_H

#include "Sektor.h"

#define PLAYER_SAVE_VERSION 1


class Player {
public:
    Player();
    Player(const Player& orig);
    virtual ~Player();
    
    DRReturn init();
    void exit();
    
    
private:
    
    DRReturn loadFromFile(const char* file = "data/player.sav");
    DRReturn saveIntoFile(const char* file = "data/player.sav");
    
    u64 mServerID;
    SektorID mSektorID;
};

#endif	/* PLAYER_H */

