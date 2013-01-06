/* 
 * File:   Player.cpp
 * Author: dario
 * 
 * Created on 20. August 2011, 18:56
 */

#include "Player.h"
#include "SolarSystemSektor.h"
#include "time.h"
#include "Camera.h"

Player::Player()
: mServerID(0), mSektorID(0), mCamera(NULL), mPosition(), mCameraFOV(45), mSeed(0), mCurrentSektor(NULL),
  mCurrentSpeed(0.0, M)
{
}

Player::Player(const Player& orig) 
: mServerID(orig.mServerID), mSektorID(orig.mSektorID), mCamera(orig.mCamera), mPosition(orig.mPosition),
  mCameraFOV(orig.mCameraFOV), mSeed(orig.mSeed), mCurrentSektor(orig.mCurrentSektor),
  mCurrentSpeed(orig.mCurrentSpeed)
{
}

Player::~Player() {
}

DRReturn Player::init()
{
    bool newPlayer = false;
	if(!mCamera)
		mCamera = new Camera;
    mSeed = static_cast<int>(time(NULL));//SDL_GetTicks();
    if(loadFromFile())
    {
        DRRandom::seed(mSeed / SDL_GetTicks());
        mServerID = Server::createNewServer();
        newPlayer = true;
        mSeed = DRRandom::core2_rand();
    }
    //Server::getServer(mServerID);
    //mCurrentSektor = Server::getServer(mServerID)->getRootSektor();
    DREngineLog.writeToLog("Player Seed: %d", mSeed);
    
    // position, radius, id, parent
    RootSektor* root = Server::getServer(mServerID)->getRootSektor();
    DRRandom::seed(mSeed);
    mCurrentSektor = new SolarSystemSektor(Vector3Unit(0.0), Unit(100, AE), mSeed, root);
    root->addSektor(mCurrentSektor, mSeed);
    
    if(!mCurrentSektor) LOG_ERROR("no memory for sektor", DR_ERROR);
    Vector3Unit position(DRRandom::rVector3(1.0f), AE);
    position = position.normalize();
    position = position * Unit(0.2f, AE);
   // position.print("Planeten position");
    
    if(newPlayer)
    {
        mCurrentSektor->moveAll(0.0f, mCamera);
        mCameraFOV = 45.0f;
    }
    Sektor* camSektor = root->getSektorByPath(mCameraSektorPath);
    if(camSektor)
        mCamera->setCurrentSektor(camSektor);
    else
        mCamera->setCurrentSektor(mCurrentSektor);
    
    mCamera->updateSektorPath();
    //DRLog.writeToLog("[Player::init] camera sektor path after load: %s", mCamera->getCurrentSektor()->getSektorPathName().data());
    //mCamera->getSektorPosition().print("[Player::init] camera position after load", true);
   
    return DR_OK;
}

void Player::exit()
{
    saveIntoFile();
    //DR_SAVE_DELETE(mCurrentSektor);
    DR_SAVE_DELETE(mCamera);
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
    f.read(&mSeed, sizeof(int), 1);
    f.read(&mServerID, sizeof(long long), 1);
    f.read(&mSektorID, sizeof(SektorID), 1);    
    f.read(&mCameraFOV, sizeof(float), 1);
    
    DRVector3 temp[4];
    for(int i = 0; i < 4; i++)
        f.read(temp[i].c, sizeof(float), 3);
    
    mCamera->setPosition(temp[0]);
    mCamera->setAxis(temp[1], temp[2], temp[3]);
    
    f.read(&mPosition, sizeof(Vector3Unit), 1);
    Vector3Unit t;
    f.read(&t, sizeof(Vector3Unit), 1);
    mCamera->setSektorPosition(t);
    
    int size = 0;
    f.read(&size, sizeof(int), 1);
    //printf("load Sektor, size: %d\n", size);
    std::stringstream s(std::stringstream::in|std::stringstream::out);
    s << "[Player::loadFromFile] sektorpath: ./data/";
    for(int i = 0; i < size; i++)
    {
        SektorID tempID = 0;
        f.read(&tempID, sizeof(SektorID), 1);
        //printf("%d id: %uld", i, (u64)tempID);
        mCameraSektorPath.push_back(tempID);
        s << "_" << tempID << "/";
    }    
    s << std::endl;
   // DRLog.writeToLog(s.str());
    
    f.close();
    LOG_INFO("Player sucessfull loaded");
    
    return DR_OK;
}

DRReturn Player::saveIntoFile(const char* file)
{
    DRFile f(file, "wb");
    if(!f.isOpen()) LOG_ERROR("Fehler beim Öffnen der Datei", DR_ERROR);
    
    int version = PLAYER_SAVE_VERSION;
    
    f.write(&version, sizeof(int), 1);
    f.write(&mSeed, sizeof(int), 1);
    f.write(&mServerID, sizeof(long long), 1);
    f.write(&mSektorID, sizeof(SektorID), 1);    
    f.write(&mCameraFOV, sizeof(float), 1);
    f.write(mCamera->getPosition().c, sizeof(float), 3);
    f.write(mCamera->getXAxis().c, sizeof(float), 3);
    f.write(mCamera->getYAxis().c, sizeof(float), 3);
    f.write(mCamera->getZAxis().c, sizeof(float), 3);
    f.write(&mPosition, sizeof(Vector3Unit), 1);
    Vector3Unit temp = mCamera->getSektorPosition();
    f.write(&temp, sizeof(Vector3Unit), 1);
    
    const std::vector<SektorID>& sektorPath = mCamera->getCurrentSektor()->getSektorPath();
    //DRLog.writeToLog("[Player::saveIntoFile] camera sektor path by save: %s", mCamera->getCurrentSektor()->getSektorPathName().data());
    //temp.print("[Player::saveIntoFile] camera position", true);
    int size = sektorPath.size();
    f.write(&size, sizeof(int), 1);
    for(int i = 0; i < size; i++)
        f.write(&sektorPath[i], sizeof(SektorID), 1);
    
    f.close();
    LOG_INFO("Player sucessfull saved");
    
    return DR_OK;
}
