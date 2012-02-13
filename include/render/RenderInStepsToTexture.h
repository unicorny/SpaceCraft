#ifndef __SC_RENDER_IN_STEPS_TO_TEXTURE__
#define __SC_RENDER_IN_STEPS_TO_TEXTURE__

class RenderInStepsToTexture
{
public:
    // frame buffer, will be called in init and in step
    RenderInStepsToTexture();
    ~RenderInStepsToTexture();
    
    // reset values, set step size
    DRReturn init(float stepSize, float clippingBorder[4], Texture* texture);
    //! erstellt eine neue Textur mit denselben Einstellungen, jedoch wird die texturgröße geändert
    DRReturn reinit(Texture* texture);
    // render with current step, add step
    DRReturn step();
    
    DRReturn saveImageToFile(const char* path);
    
    // called every step
    virtual DRReturn renderStuff() = 0;        
    
    __inline__ bool isFinished() {return mFinished;}
    __inline__ Texture* getTextur() {return mTexture;}
    
protected:
    float       mClippingBorder[4]; //left, rigt, bottom, top
    DRVector2   mTextureSize;    
    Texture*     mTexture;
	DRMatrix	mProjectionMatrix;
    
private:
    DRVector2   mCursorIndex;
    float       mStepSize;
    u8          mIndexStepMode; 
    short       mCursorMaxCount;
    short       mCursorCurrentCount;
    bool        mFinished;
};

#endif //__SC_RENDER_IN_STEPS_TO_TEXTURE__