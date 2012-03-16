#ifndef __SC_RENDER_IN_STEPS_TO_TEXTURE__
#define __SC_RENDER_IN_STEPS_TO_TEXTURE__

#include "Texture.h"

class RenderInStepsToTexture: public DRIResource
{
public:
    // frame buffer, will be called in init and in step
    RenderInStepsToTexture();
    ~RenderInStepsToTexture();
    
    // reset values, set step size
    DRReturn init(float stepSize, float clippingBorder[4], TexturePtr texture);
    //! erstellt eine neue Textur mit denselben Einstellungen, jedoch wird die texturgröße geändert
    DRReturn reinit(TexturePtr texture);
    // render with current step, add step
    DRReturn step();
    
    DRReturn saveImageToFile(const char* path);
    
    // called every step
    virtual DRReturn renderStuff() = 0;        
    
    __inline__ bool isFinished() {return mFinished;}
    __inline__ TexturePtr getTextur() {return mTexture;}
    
    virtual const char* getResourceType() const {return "RenderInStepsToTexture";}
    
protected:
    float       mClippingBorder[4]; //left, rigt, bottom, top
    DRVector2   mTextureSize;    
    TexturePtr     mTexture;
    DRMatrix	mProjectionMatrix;
    
private:
    DRVector2   mCursorIndex;
    float       mStepSize;
    u8          mIndexStepMode; 
    short       mCursorMaxCount;
    short       mCursorCurrentCount;
    bool        mFinished;
};

typedef DRResourcePtr<RenderInStepsToTexture> RenderInStepsToTexturePtr;

#endif //__SC_RENDER_IN_STEPS_TO_TEXTURE__