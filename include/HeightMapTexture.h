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
class HeightMapTexture;

typedef DRSimpleResourcePtr<HeightMapTexture> HeightMapTexturePtr;

class HeightMapTexture
{
    friend class HeightMapLoader;
public:
    //! \param stepSize bytes to load je frame from texture (like by DRSaveTexture)
    HeightMapTexture(int stepSize);
    virtual ~HeightMapTexture();
    
    void load(DRTexturePtr source);
    
    __inline__ float getMaxHeight() {return mMaxHeight;}
    __inline__ float getMaxGradient() {return mMaxGradient;}
        
protected:
    void copyPixelData(u8* data, DRVector2i size);
    HeightMapTexturePtr* mThis;
    float*          mPixelHeightData;
    int             mLoadStepSize;
    DRVector2i      mSize;
    SDL_sem *       mPixelCopySemaphore;
    
    float           mMaxHeight;
    float           mMaxGradient;    
};



class HeightMapLoader : public DRSaveTexture
{
public:
    HeightMapLoader(DRTexturePtr source, int stepSize, HeightMapTexturePtr* parent);
    ~HeightMapLoader();
    virtual DRReturn saveImage();
    
private:
    HeightMapTexturePtr* mParent;    
    
};
#endif //__SC_HEIGHT_MAP_TEXTURE_H
