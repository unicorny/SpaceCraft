/* 
 * File:   Player.h
 * Author: dario
 *
 * Created on 20. August 2011, 18:56
 */

#ifndef __SC_PLAYER_H
#define	__SC_PLAYER_H

#include "Sektor.h"

#define PLAYER_SAVE_VERSION 5


class Player {
public:
    Player();
    Player(const Player& orig);
    virtual ~Player();
    
    DRReturn init();
    void exit();
    
    // setter und getter
    __inline__ void setCameraFOV(float newFOV) {mCameraFOV = newFOV;}
    __inline__ float getCameraFOV() {return mCameraFOV;}
    __inline__ Camera* getCamera() {return &mCamera;}
    __inline__ Sektor* getSektor() {return mCurrentSektor;}
    
private:
    
    DRReturn loadFromFile(const char* file = "data/player.sav");
    DRReturn saveIntoFile(const char* file = "data/player.sav");
    
    // position
    u64 mServerID;
    SektorID mSektorID;
    Camera mCamera;
    // current position of player in current sektor in sektor coordinates (for exapmle, in AE)
    Vector3Unit mPosition;
    
    //einstellungen
    //! kamera winkel in Grad
    float mCameraFOV;
    int mSeed;
    
    
    //runtime (not to save)
    Sektor* mCurrentSektor;
};

#endif	/* PLAYER_H */

