#ifndef __SC_RENDER_TO_TEXTURE__
#define __SC_RENDER_TO_TEXTURE__

#include "main.h"

class RenderToTexture : public DRIResource
{
public:
     // frame buffer, will be called in init and in step
    RenderToTexture();
    virtual ~RenderToTexture();
    
    // reset values, set step size
    virtual DRReturn init(DRTexturePtr texture);

    // render with current step, add step
    virtual DRReturn step();
    
    virtual bool isFinished() {return mFinished == 2;}
    
    // called every step
    virtual DRReturn renderStuff() = 0;        
    
    __inline__ DRTexturePtr getTextur() {return mTexture;}
    
    virtual const char* getResourceType() const {return "RenderToTexture";}
    
    virtual void setFilenameToSave(DRString path) {mFileNameToSave = path;}    
    
    virtual bool less_than(DRIResource& b) const
    {
        //printf("[RenderInStepsToTexture::operator <]\n");
        return this->mTexture->getId() < dynamic_cast<RenderToTexture&>(b).getTextur()->getId();
    }
    
protected:
    DRVector2i  mTextureSize;    
    DRTexturePtr  mTexture;
    DRMatrix	mProjectionMatrix;
    u8           mFinished;
    DRString     mFileNameToSave;
    
private:
     
};

typedef DRResourcePtr<RenderToTexture> RenderToTexturePtr;

#endif //__SC_RENDER_TO_TEXTURE__