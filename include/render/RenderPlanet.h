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
//class RenderNoisePlanetToTexture;

class RenderPlanet : public RenderSektor
{
public:
    RenderPlanet(SektorID seed, DRString texturePath);

    virtual ~RenderPlanet();
    
    virtual DRReturn render(float fTime, Camera* cam);
    void setTexturePath(DRString texturePath) {mTexturePath = texturePath;}
    
protected:
    RenderPlanet(SektorID seed, float theta, 
                 float cameraDistance, const DRMatrix& rotation, DRString texturePath);
    DRReturn init(SektorID seed, float theta, 
                 float cameraDistance, const DRMatrix& rotation,
                 const char* vertexShader, const char* fragmentShader, int textureSize, DRString texturePath);
    
    DRReturn generateAndBindTexture();
    DRString getPathAndFilename();
        
    RenderNoisePlanetToTexture* mTextureRenderer;
	Texture*						mTexture;
	Texture*						mPreviewTextur;
    short                       mInitalized;
    DRString                    mTexturePath;
    
private:    
    
    
};


#endif	/* __SC_RENDER_PLANET_H */

