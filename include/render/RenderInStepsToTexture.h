#ifndef __SC_RENDER_IN_STEPS_TO_TEXTURE__
#define __SC_RENDER_IN_STEPS_TO_TEXTURE__

#include "main.h"

class RenderInStepsToTexture: public DRIResource
{
public:
    // frame buffer, will be called in init and in step
    RenderInStepsToTexture();
    ~RenderInStepsToTexture();
    
    // reset values, set step size
    DRReturn init(int stepSize, DRReal clippingBorder[4], DRTexturePtr texture);
    //! erstellt eine neue Textur mit denselben Einstellungen, jedoch wird die texturgröße geändert
    DRReturn reinit(DRTexturePtr texture);
    // render with current step, add step
    DRReturn step();
    
    DRReturn saveImageToFile(const char* path);
    
    // called every step
    virtual DRReturn renderStuff() = 0;        
    
    __inline__ bool isFinished() {return mFinished;}
    __inline__ DRTexturePtr getTextur() {return mTexture;}
    
    virtual const char* getResourceType() const {return "RenderInStepsToTexture";}
    
    __inline__ void setCurrentDistance(DRReal distance) {mDistance = distance;}
    
    virtual bool less_than(DRIResource& b) const
    {
        //printf("[RenderInStepsToTexture::operator <]\n");
        return this->mDistance < dynamic_cast<RenderInStepsToTexture&>(b).mDistance;
    }
    
protected:
    float       mClippingBorder[4]; //left, rigt, bottom, top
    DRVector2i  mTextureSize;    
    DRTexturePtr  mTexture;
    DRMatrix	mProjectionMatrix;
    
private:
    DRVector2i  mCursorIndex;
    int         mStepSize;
    u8          mIndexStepMode; 
    short       mCursorMaxCount;
    short       mCursorCurrentCount;
    bool        mFinished;
    
    DRReal       mDistance; //distance to viewer, for priority list
};

typedef DRResourcePtr<RenderInStepsToTexture> RenderInStepsToTexturePtr;

#endif //__SC_RENDER_IN_STEPS_TO_TEXTURE__