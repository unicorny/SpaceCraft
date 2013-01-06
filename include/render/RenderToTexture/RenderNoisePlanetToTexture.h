#ifndef __SC_RENDER_NOISE_PLANET_TO_TEXTURE__
#define __SC_RENDER_NOISE_PLANET_TO_TEXTURE__

#include "RenderInStepsToTexture.h"
#include "ShaderProgram.h"

class DRGeometrieHeightfield;
struct PlanetNoiseParameter;

class RenderNoisePlanetToTexture : public RenderInStepsToTexture
{
public:
    RenderNoisePlanetToTexture(ShaderProgramParameter shader[5], const PlanetNoiseParameter* noiseParameter);
    virtual ~RenderNoisePlanetToTexture();
    
    //__inline__ void update(float patchScaling) {mPatchScaling = patchScaling;}
    virtual DRReturn init(int stepSize, DRVector3 translate, float patchScaling, DRTexturePtr texture, const DRMatrix& rotation = DRMatrix::identity());
    
    virtual DRReturn reinit(DRTexturePtr texture);
    virtual DRReturn renderStuff();
    virtual DRReturn step();
    
    virtual void setFilenameToSave(DRString path);
    
    __inline__ const DRMatrix& getRotationsMatrix() {return mRotation;}
    __inline__ float getPatchScaling() {return mPatchScaling;}
        
protected:
private:
    ShaderProgramPtr mShader[5];
    u8             mShaderCursor;
    DRGeometrieHeightfield* mRenderGrid;
    const PlanetNoiseParameter*      mNoiseParameter;
    float         mPatchScaling;
    DRVector3     mTranslate;
    DRMatrix      mRotation;
    DRTexturePtr  mPermTexture;
    DRTexturePtr  maTextures[5];
    
    //debugging
    DRString        mFilename;
    
    
};


#endif //__SC_RENDER_NOISE_PLANET_TO_TEXTURE__