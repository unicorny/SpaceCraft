#ifndef __SC_RENDER_IN_STEPS_TO_TEXTURE__
#define __SC_RENDER_IN_STEPS_TO_TEXTURE__

class RenderInStepsToTexture
{
public:
    // frame buffer, will be called in init and in step
    RenderInStepsToTexture();
    ~RenderInStepsToTexture();
    
    // reset values, set step size
    DRReturn init(float stepSize, DRVector2 textureSize, float clippingBorder[4], GLuint textureID);
    // render with current step, add step
    DRReturn step();
    
    DRReturn saveImageToFile(const char* path);
    
    // called every step
    virtual DRReturn renderStuff() = 0;        
    
    __inline__ bool isFinished() {return mFinished;}
    __inline__ GLuint getTextureID() {return mTextureID;}
    
protected:
    float       mClippingBorder[4]; //left, rigt, bottom, top
    DRVector2   mTextureSize;    
    GLuint      mTextureID;
    
private:
    DRVector2   mCursorIndex;
    float       mStepSize;
    u8          mIndexStepMode; 
    short       mCursorMaxCount;
    short       mCursorCurrentCount;
    bool        mFinished;
};

#endif //__SC_RENDER_IN_STEPS_TO_TEXTURE__