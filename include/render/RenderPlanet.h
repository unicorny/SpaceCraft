/* 
 * File:   RenderPlanet.h
 * Author: Dario
 *
 * Created on 6. November 2011, 14:48
 */

#ifndef __SC_RENDER_PLANET_H
#define	__SC_RENDER_PLANET_H

#include "noiseutils.h"
#include "GenerateNoisePlanet.h"

class PlanetHeightValues: public DRHeightValueStorage
{
public:
<<<<<<< HEAD
    RenderPlanet(SektorID seed, DRString texturePath, const PlanetNoiseParameter* planetNoiseParameter);
=======
   PlanetHeightValues(GenerateNoisePlanet* noisePlanet = NULL);
   virtual ~PlanetHeightValues();
   virtual float getHeightValue(DRVector3& position);
   virtual DRColor getColorValue(const float height);
   
   __inline__ void setNoisePlanet(GenerateNoisePlanet* noisePlanet) {mNoisePlanet = noisePlanet;}
   __inline__ void AddGradientPoint (double gradientPos, const noise::utils::Color& gradientColor) {mColor.AddGradientPoint(gradientPos, gradientColor);}
   __inline__ void ClearGradientPoints() {mColor.Clear();}
   
private:
    //! \return true if value could be found, else false
    noise::utils::GradientColor mColor;
    GenerateNoisePlanet* mNoisePlanet;    
    
};
>>>>>>> c1a3e92256268443cee33f382589cb05f672be4a

class RenderPlanet : public RenderSektor
{
public:
    RenderPlanet(GenerateNoisePlanet* noiseGenerator);
    virtual ~RenderPlanet();
    
    virtual DRReturn render(float fTime, Camera* cam);
    
<<<<<<< HEAD
    RenderNoisePlanetToTexture* getRenderNoisePlanetToTexture();
    
protected:
    RenderPlanet(SektorID seed, DRVector3 translate, float patchScaling, const DRMatrix& rotation, DRString texturePath, const PlanetNoiseParameter* planetNoiseParameter);
    DRReturn init(SektorID seed, DRVector3 translate, float patchScaling, const DRMatrix& rotation, 
                  const char* vertexShader, const char* fragmentShader, int textureSize, DRString texturePath,
                  const PlanetNoiseParameter* planetNoiseParameter);
    
    DRReturn generateAndBindTexture();
    DRString getPathAndFilename();
        
    RenderInStepsToTexturePtr   mTextureRenderer;
    DRTexturePtr		mTexture;
    DRTexturePtr		mPreviewTextur;
    short                       mInitalized;
    DRString                    mTexturePath;
=======
    static int TextureGenerateThread(void* data);
    virtual DRReturn updateTexture(DRVector2 newSize, int (*function)(void*), bool waitToComplete = false);
    
protected:
    PlanetHeightValues*         mHeights;
    GenerateNoisePlanet*        mNoiseGenerator;
    DRTextur*                   mPlanetTexture;
    DRGeometrieIcoSphere*       mGeometrie;
>>>>>>> c1a3e92256268443cee33f382589cb05f672be4a
    
private:    
    SDL_Thread*				mUpdateTextureThread;
    SDL_sem *				mUpdateTextureThreadSemaphore;
    SDL_mutex*                          mUpdateTextureMutex;
    
    DRColor*                            mTextureTempPoints;
    DRVector2                           mTextureSize;
};


#endif	/* __SC_RENDER_PLANET_H */

