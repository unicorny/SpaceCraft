#ifndef __SC_RENDER_NOISE_PLANET_TO_TEXTURE__
#define __SC_RENDER_NOISE_PLANET_TO_TEXTURE__

#include "RenderInStepsToTexture.h"
#include "ShaderProgram.h"

class DRGeometrieHeightfield;
struct PlanetNoiseParameter;

class RenderNoisePlanetToTexture : public RenderInStepsToTexture
{
public:
    RenderNoisePlanetToTexture(const char* vertexShaderName, const char* fragmentShaderName, const PlanetNoiseParameter* noiseParameter);
    ~RenderNoisePlanetToTexture();
    
    //__inline__ void update(float patchScaling) {mPatchScaling = patchScaling;}
    virtual DRReturn init(int stepSize, DRVector3 translate, float patchScaling, DRTexturePtr texture, const DRMatrix& rotation = DRMatrix::identity());
    
    virtual DRReturn renderStuff();
    
    __inline__ const DRMatrix& getRotationsMatrix() {return mRotation;}
    __inline__ float getPatchScaling() {return mPatchScaling;}
        
protected:
private:
    ShaderProgram* mShader;
    DRGeometrieHeightfield* mRenderGrid;
    const PlanetNoiseParameter*      mNoiseParameter;
    float         mPatchScaling;
    DRVector3     mTranslate;
    DRMatrix      mRotation;
    DRTexturePtr mPermTexture;
    
    
};


#endif //__SC_RENDER_NOISE_PLANET_TO_TEXTURE__