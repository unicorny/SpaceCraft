/* 
 * File:   HeightMapTexture.h
 * Author: Dario
 *
 * Created on 12. August 2012, 08:09
 * 
 * 
 */

#ifndef __SC_HEIGHT_MAP_TEXTURE_H
#define	__SC_HEIGHT_MAP_TEXTURE_H

#include "main.h"

class HeightMapLoader;

class HeightMapTexture
{
    friend HeightMapLoader;
public:
    //! \param stepSize bytes to load je frame from texture (like by DRSaveTexture)
    HeightMapTexture(int stepSize);
    ~HeightMapTexture();
    
    void load(DRTexturePtr source);
    
protected:
    void copyPixelData(u8* data, DRVector2i size);
    float*          mPixelHeightData;
    int             mLoadStepSize;
    DRVector2i      mSize;
    bool            mState;
    SDL_mutex*      mPixelCopyMutex;
    
    float           mMaxHeight;
    float           mMaxGradient;    
};


class HeightMapLoader : public DRSaveTexture
{
public:
    HeightMapLoader(DRTexturePtr source, int stepSize, HeightMapTexture* parent);
    ~HeightMapLoader();
    virtual DRReturn saveImage();
    
private:
    HeightMapTexture* mParent;    
    
};
#endif __SC_HEIGHT_MAP_TEXTURE_H