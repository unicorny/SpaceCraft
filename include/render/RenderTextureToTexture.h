#ifndef __SC_RENDER_TEXTURE_TO_TEXTURE__
#define __SC_RENDER_TEXTURE_TO_TEXTURE__

#include "RenderToTexture.h"
#include "ShaderProgram.h"

class RenderTextureToTexture : public RenderToTexture
{
public:
     // frame buffer, will be called in init and in step
    RenderTextureToTexture();
    ~RenderTextureToTexture();
    
    // reset values, set step size
    virtual DRReturn init(DRTexturePtr targetTexture, DRTexturePtr sourceTexture, ShaderProgram* shader);
    
     //virtual bool isFinished() {return mFinished == 2;}
    
    // called every step
    virtual DRReturn renderStuff();        
    
    __inline__ DRTexturePtr getTextur() {return mTexture;}
    
    virtual const char* getResourceType() const {return "RenderTextureToTexture";}
      
    
protected:
    
    
private:
    DRTexturePtr        mSourceTexture;
    ShaderProgram*      mShader;            
};

#endif //__SC_RENDER_TO_TEXTURE__