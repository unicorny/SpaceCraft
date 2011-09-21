/* 
 * File:   Player.cpp
 * Author: dario
 * 
 * Created on 20. August 2011, 18:56
 */

#include "main.h"

Player::Player()
: mServerID(0), mSektorID(0), mPosition(), mCameraFOV(45), mCurrentSektor(NULL)
{
}

Player::Player(const Player& orig) 
: mServerID(orig.mServerID), mSektorID(orig.mSektorID), mPosition(), mCurrentSektor(orig.mCurrentSektor)
{
}

Player::~Player() {
}

DRReturn Player::init()
{
    if(loadFromFile())
        mServerID = Server::createNewServer();
    
    mCurrentSektor = new Sektor(0);
    if(!mCurrentSektor) LOG_ERROR("no memory for sektor", DR_ERROR);
    Vector3Unit position(Unit(0, LIGHTYEAR));
    srand(SDL_GetTicks());
    int seed = rand();
    Unit radius(DRRandom::rDouble(72000, 1000), KM);
    mCurrentSektor->addStellarBody(new Planet(radius, position, seed, mCurrentSektor));
    return DR_OK;
}

void Player::exit()
{
    saveIntoFile();
    DR_SAVE_DELETE(mCurrentSektor);
}

DRReturn Player::loadFromFile(const char* file)
{
    //FILE* f = fopen(file);
    DRFile f(file, "rb");
    if(!f.isOpen()) LOG_ERROR("Fehler beim Öffnen der Datei, ist sie vielleicht nicht vorhanden?", DR_ERROR);
    
    int version = 0;
    f.read(&version, sizeof(int), 1);
    if( version != PLAYER_SAVE_VERSION)
        LOG_ERROR("Fehler, falsche Version!", DR_ERROR);
    
    f.read(&mServerID, sizeof(long long), 1);
    f.read(&mSektorID, sizeof(SektorID), 1);    
    f.read(&mCameraFOV, sizeof(float), 1);
    
    DRVector3 temp[4];
    for(int i = 0; i < 4; i++)
        f.read(temp[i].c, sizeof(float), 3);
    
    mCamera.setPosition(temp[0]);
    mCamera.setAxis(temp[1], temp[2], temp[3]);
    
    f.read(&mPosition, sizeof(Vector3Unit), 1);
    Vector3Unit t;
    f.read(&t, sizeof(Vector3Unit), 1);
    mCamera.setAbsPosition(t);
    
    f.close();
    
    return DR_OK;
}

DRReturn Player::saveIntoFile(const char* file)
{
    DRFile f(file, "wb");
    if(!f.isOpen()) LOG_ERROR("Fehler beim Öffnen der Datei", DR_ERROR);
    
    int version = PLAYER_SAVE_VERSION;
    
    f.write(&version, sizeof(int), 1);
    f.write(&mServerID, sizeof(long long), 1);
    f.write(&mSektorID, sizeof(SektorID), 1);    
    f.write(&mCameraFOV, sizeof(float), 1);
    f.write(mCamera.getPosition().c, sizeof(float), 3);
    f.write(mCamera.getXAxis().c, sizeof(float), 3);
    f.write(mCamera.getYAxis().c, sizeof(float), 3);
    f.write(mCamera.getZAxis().c, sizeof(float), 3);
    f.write(&mPosition, sizeof(Vector3Unit), 1);
    Vector3Unit temp = mCamera.getAbsPosition();
    f.write(&temp, sizeof(Vector3Unit), 1);
    
    f.close();
    LOG_INFO("Player sucessfull saved");
    
    return DR_OK;
}
