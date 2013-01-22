/* 
 * File:   RenderPlanet.h
 * Author: Dario
 *
 * Created on 6. November 2011, 14:48
 */

#ifndef __SC_RENDER_PLANET_H
#define	__SC_RENDER_PLANET_H

#include "Sektor.h"
#include "RenderNoisePlanetToTexture.h"
#include "HeightMapTexture.h"
//class RenderNoisePlanetToTexture;

class RenderPlanet : public RenderSektor
{
public:
    RenderPlanet(DRString texturePath, const PlanetNoiseParameter* planetNoiseParameter);

    virtual ~RenderPlanet();
    
    DRReturn init(DRVector3 translate, float patchScaling, const DRMatrix& rotation,
                  int textureSize, DRString texturePath,
                  const PlanetNoiseParameter* planetNoiseParameter);
    
    virtual DRReturn render(float fTime, Camera* cam);
    void setTexturePath(DRString texturePath) {mTexturePath = texturePath;}
    
    RenderNoisePlanetToTexture* getRenderNoisePlanetToTexture();
    __inline__ DRTexturePtr getTexture() {return mTexture;}
    __inline__ HeightMapTexture* getHeightMap() {return mHeightMap;}
    
    __inline__ virtual bool isFinishLoading() {return 4 == mInitalized;}
    __inline__ bool isOneStepBeforeFinish() {return 3 == mInitalized;}
    __inline__ virtual bool isErrorOccured() {return -1 == mInitalized;}
    __inline__ virtual bool isInitalized() {return 0 < mInitalized;}
    
    
    DRReturn generateTexture();
    
protected:
    RenderPlanet(DRVector3 translate, float patchScaling, const DRMatrix& rotation, DRString texturePath, const PlanetNoiseParameter* planetNoiseParameter);
    RenderPlanet();
    
    
    DRReturn bindTexture();
    DRReturn generateFinalTexture();
    DRString getPathAndFilename();
        
    RenderToTexturePtr          mTextureRenderer;
    float                       mSeaLevelInMetres;
    DRTexturePtr                mHeightTexture;
    DRTexturePtr                mTexture;
    short                       mInitalized; // 0 = nothing, 1  = preview, 2 = rendered, 3 = load texture, 4 = texture loading finish, -1 fehler
    DRString                    mTexturePath;    
    HeightMapTexture*           mHeightMap;
    DRString                    mVertexShader;
    
    static DRString             mFragmentShader[];
    
private:    
    
    
};


#endif	/* __SC_RENDER_PLANET_H */

