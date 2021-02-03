#include "RenderInStepsToTexture.h"
#include "GlobalRenderer.h"

RenderInStepsToTexture::RenderInStepsToTexture() 
: RenderToTexture(), mStepSize(0), mIndexStepMode(0), mDistance(-1.0f)
{
    
}

RenderInStepsToTexture::~RenderInStepsToTexture()
{
    
}

DRReturn RenderInStepsToTexture::init(int stepSize, DRTexturePtr texture)
{
    if(RenderToTexture::init(texture)) 
        LOG_ERROR("error by init render to texture", DR_ERROR);
        
    mStepSize = stepSize;
    //mTextureSize = textureSize;
    mCursorIndex = mTextureSize/2 - DRVector2i(static_cast<int>(stepSize)/2);
    mIndexStepMode = 0;
    mCursorMaxCount = 1;
    mCursorCurrentCount = 0;
    
    return DR_OK;
}

DRReturn RenderInStepsToTexture::reinit(DRTexturePtr texture)
{
    if(RenderToTexture::init(texture)) 
        LOG_ERROR("error by init render to texture", DR_ERROR);
    
   // mTextureSize *= textureSizeScalar;
 //   mTextureSize = textureSize;
    mCursorIndex = mTextureSize/2 - DRVector2i(static_cast<int>(mStepSize)/2);
    mIndexStepMode = 0;
    mCursorMaxCount = 1;
    mCursorCurrentCount = 0;
    
    return DR_OK;
}

DRReturn RenderInStepsToTexture::step()
{
     if(mCursorIndex > mTextureSize || mCursorIndex < DRVector2i(0))
     {
        mFinished = 1;
        if(mFileNameToSave.size() > 0)
        {
            //DRLog.writeToLog("Dateiname fur Textur: %s", filename.data());
            //mTextureRenderer->saveImageToFile(getPathAndFilename().data());
            DRTextureManager::Instance().saveTexture(mTexture, mFileNameToSave.data(),
                GlobalRenderer::Instance().getTextureRenderStepSize()*
                GlobalRenderer::Instance().getTextureRenderStepSize()*16);
                // */
        }
        return DR_OK;
     }
    
    //mFrameBuffer->bindToRender();
	 setupFrameBuffer();
    
    //render stuff
    if(mCursorIndex == mTextureSize/2 - DRVector2i(static_cast<int>(mStepSize)/2))
    {
        glClearColor(0.0, 0.0, 0.0, 0);
        glClear (GL_COLOR_BUFFER_BIT);   
    }
    
    glColor3f(0.0f, 1.0f, 0.0f);    
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
           
    //mCursorIndex / mTextureSize
    /*DRVector2 clippingSize = DRVector2(mClippingBorder[1]-mClippingBorder[0],
                                       mClippingBorder[3]-mClippingBorder[2]);*/
    DRVector2 clippingSize = DRVector2(-2.0f);
    DRVector2 cursor = DRVector2(mCursorIndex) / DRVector2(mTextureSize) * clippingSize + DRVector2(1.0f);
    DRVector2 halfStepSize = (DRVector2(static_cast<DRReal>(mStepSize)) / DRVector2(mTextureSize) * clippingSize) * 0.5f;
    
    //Reseten der Matrixen
    glViewport(mCursorIndex.x-mStepSize/2, mCursorIndex.y-mStepSize/2, mStepSize, mStepSize);
    
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
       
    // left, right, bottom, top, near, far    
//    glOrtho(cursor.x-halfStepSize.x, cursor.x+halfStepSize.x, cursor.y-halfStepSize.y, cursor.y+halfStepSize.y, -1.0, 1.0);
	//glMultMatrixf(DRMatrix::ortho_projection(cursor.x-halfStepSize.x, cursor.x+halfStepSize.x, cursor.y-halfStepSize.y, cursor.y+halfStepSize.y, -1.0, 1.0));
	mProjectionMatrix = DRMatrix::ortho_projection(cursor.x-halfStepSize.x, cursor.x+halfStepSize.x, cursor.y-halfStepSize.y, cursor.y+halfStepSize.y, -1.0, 1.0);
  //  printf("left: %f, right: %f, bottom: %f, top: %f, pixel cursor: %f, %f, stepSize: %f\n", 
    //        cursor.x-halfStepSize.x, cursor.x+halfStepSize.x, cursor.y-halfStepSize.y, cursor.y+halfStepSize.y, mCursorIndex.x, mCursorIndex.y, mStepSize);
    //glOrtho(-1.0, 1.0, 1.0, -1.0, -1.0, 1.0);
    
    //glMatrixMode(GL_MODELVIEW);          // Select the modelview matrix
    //glLoadIdentity();                    // Reset (init) the modelview matrix
    DRReturn ret = renderStuff();
    
    //clean up
   // mFrameBuffer->unbind();    
    if(ret) LOG_ERROR("Fehler bei RenderInStepsToTexture::renderStuff call", DR_ERROR);
    DRGrafikError("RenderInStepsToTexture::step: Fehler bei render to texture");
    
    switch(mIndexStepMode)
    {
    case 0: mCursorIndex.x += mStepSize; break;//rechts
    case 1: mCursorIndex.y += mStepSize; break;//unten
    case 2: mCursorIndex.x -= mStepSize; break;//links
    case 3: mCursorIndex.y -= mStepSize; break;//oben
    }
    mCursorCurrentCount++;
    if(mCursorCurrentCount >= mCursorMaxCount)
    {
        mIndexStepMode++;
        if(mIndexStepMode >= 4) mIndexStepMode = 0;
        mCursorCurrentCount = 0;
        if(2 == mIndexStepMode || 0 == mIndexStepMode)
            mCursorMaxCount++;
    }
    
    return DR_OK;
}
