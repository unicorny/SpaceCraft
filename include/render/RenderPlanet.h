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
    RenderPlanet(SektorID seed, DRString texturePath, const PlanetNoiseParameter* planetNoiseParameter);

    virtual ~RenderPlanet();
    
    virtual DRReturn render(float fTime, Camera* cam);
    void setTexturePath(DRString texturePath) {mTexturePath = texturePath;}
    
    RenderNoisePlanetToTexture* getRenderNoisePlanetToTexture();
    __inline__ DRTexturePtr getTexture() {return mTexture;}
    __inline__ HeightMapTexture* getHeightMap() {return mHeightMap;}
    
    __inline__ bool isFinishLoading() {return 4 == mInitalized;}
    __inline__ bool isErrorOccured() {return -1 == mInitalized;}
    
    DRReturn generateTexture();
    
protected:
    RenderPlanet(SektorID seed, DRVector3 translate, float patchScaling, const DRMatrix& rotation, DRString texturePath, const PlanetNoiseParameter* planetNoiseParameter);
    DRReturn init(SektorID seed, DRVector3 translate, float patchScaling, const DRMatrix& rotation, 
                  ShaderProgramParameter shader[4], int textureSize, DRString texturePath,
                  const PlanetNoiseParameter* planetNoiseParameter);
    
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
    
private:    
    
    
};


#endif	/* __SC_RENDER_PLANET_H */

