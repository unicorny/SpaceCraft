/* 
 * File:   Player.cpp
 * Author: dario
 * 
 * Created on 20. August 2011, 18:56
 */

#include "main.h"

Player::Player()
: mServerID(0), mSektorID(0)
{
}

Player::Player(const Player& orig) 
: mServerID(orig.mServerID), mSektorID(orig.mSektorID)
{
}

Player::~Player() {
}

DRReturn Player::init()
{
    if(loadFromFile())
        mServerID = Server::createNewServer();
    return DR_OK;
}

void Player::exit()
{
    saveIntoFile();
}

DRReturn Player::loadFromFile(const char* file)
{
    //FILE* f = fopen(file);
    DRFile f(file);
    if(!f.isOpen()) LOG_ERROR("Fehler beim Öffnen der Datei, ist sie vielleicht nicht vorhanden?", DR_ERROR);
    
    int version = 0;
    f.read(&version, sizeof(int), 1);
    if( version != PLAYER_SAVE_VERSION)
        LOG_ERROR("Fehler, falsche Version!", DR_ERROR);
    
    f.read(&mServerID, sizeof(long long), 1);
    f.read(&mSektorID, sizeof(SektorID), 1);    
    
    f.close();
    
    return DR_OK;
}

DRReturn Player::saveIntoFile(const char* file)
{
    DRFile f(file);
    
    if(!f.isOpen()) LOG_ERROR("Fehler beim Öffnen der Datei, ist sie vielleicht nicht vorhanden?", DR_ERROR);
    
    int version = PLAYER_SAVE_VERSION;
    f.write(&version, sizeof(int), 1);
    
    f.write(&mServerID, sizeof(long long), 1);
    f.write(&mSektorID, sizeof(SektorID), 1);    
    
    f.close();
    
    return DR_OK;
}
