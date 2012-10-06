#ifndef __SC_RENDER_TO_TEXTURE__
#define __SC_RENDER_TO_TEXTURE__

#include "main.h"

class RenderToTexture : public DRIResource
{
public:
     // frame buffer, will be called in init and in step
    RenderToTexture();
    ~RenderToTexture();
    
    // reset values, set step size
    DRReturn init(DRTexturePtr texture);

    // render with current step, add step
    DRReturn step();
    
     __inline__ bool isFinished() {return mFinished == 2;}
    
    // called every step
    virtual DRReturn renderStuff() = 0;        
    
    __inline__ DRTexturePtr getTextur() {return mTexture;}
    
    virtual const char* getResourceType() const {return "RenderToTexture";}
    
    __inline__ void setFilenameToSave(DRString path) {mFileNameToSave = path;}    
    
    
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