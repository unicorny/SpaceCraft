#include "RenderToTexture.h"
#include "GlobalRenderer.h"

RenderToTexture::RenderToTexture()
: mTexture(), mFinished(0), mFileNameToSave("")
{
    
}

RenderToTexture::~RenderToTexture()
{
    
}

DRReturn RenderToTexture::init(DRTexturePtr texture)
{
	if(!texture.getResourcePtrHolder()) return DR_ZERO_POINTER;
    mTextureSize = texture->getResolution();
	if(mTextureSize.x <= 0 || mTextureSize.y <= 0)
    {
        texture->setErrorByLoading();
        DREngineLog.writeToLog("textureSize: %f %f", mTextureSize.x, mTextureSize.y);
		LOG_ERROR("textureSize is invalid", DR_ERROR);
    }
        
    mTexture = texture;
    mFinished = 0;
    
    return DR_OK;
}

DRReturn RenderToTexture::step()
{
     if(mFinished == 1)
     {
        if(mFileNameToSave.size() > 0)
        {
            //DRLog.writeToLog("Dateiname fur Textur: %s", filename.data());
            //mTextureRenderer->saveImageToFile(getPathAndFilename().data());
            DRTextureManager::Instance().saveTexture(mTexture, mFileNameToSave.data(),
                GlobalRenderer::Instance().getTextureRenderStepSize()*
                GlobalRenderer::Instance().getTextureRenderStepSize()*16);
                // */
        }
        mFinished++;
        return DR_OK;
     }
    
    
    //mFrameBuffer->bindToRender();
    
    glClearColor(0.0, 0.0, 0.0, 0);
    glClear (GL_COLOR_BUFFER_BIT);   
   
    
    glColor3f(0.0f, 1.0f, 0.0f);    
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
           
    //mCursorIndex / mTextureSize
    /*DRVector2 clippingSize = DRVector2(mClippingBorder[1]-mClippingBorder[0],
                                       mClippingBorder[3]-mClippingBorder[2]);*/
   // DRVector2 clippingSize = DRVector2(-2.0f);
   // DRVector2 cursor = DRVector2(mCursorIndex) / DRVector2(mTextureSize) * clippingSize + DRVector2(mClippingBorder[0], mClippingBorder[2]);
   // DRVector2 halfStepSize = (DRVector2(static_cast<DRReal>(mStepSize)) / DRVector2(mTextureSize) * clippingSize) * 0.5f;
    
    //Reseten der Matrixen
    glViewport(0, 0, mTextureSize.x, mTextureSize.y);
    
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
       
    // left, right, bottom, top, near, far    
//    glOrtho(cursor.x-halfStepSize.x, cursor.x+halfStepSize.x, cursor.y-halfStepSize.y, cursor.y+halfStepSize.y, -1.0, 1.0);
	//glMultMatrixf(DRMatrix::ortho_projection(cursor.x-halfStepSize.x, cursor.x+halfStepSize.x, cursor.y-halfStepSize.y, cursor.y+halfStepSize.y, -1.0, 1.0));
	mProjectionMatrix = DRMatrix::ortho_projection(0.0f, 1.0f, 0.0f, 1.0f, -1.0, 1.0);
  //  printf("left: %f, right: %f, bottom: %f, top: %f, pixel cursor: %f, %f, stepSize: %f\n", 
    //        cursor.x-halfStepSize.x, cursor.x+halfStepSize.x, cursor.y-halfStepSize.y, cursor.y+halfStepSize.y, mCursorIndex.x, mCursorIndex.y, mStepSize);
    //glOrtho(-1.0, 1.0, 1.0, -1.0, -1.0, 1.0);
    
    //glMatrixMode(GL_MODELVIEW);          // Select the modelview matrix
    //glLoadIdentity();                    // Reset (init) the modelview matrix
    DRReturn ret = renderStuff();
    
    //clean up
   // mFrameBuffer->unbind();    
    if(ret) LOG_ERROR("Fehler bei RenderToTexture::renderStuff call", DR_ERROR);
    DRGrafikError("RenderToTexture::step: Fehler bei render to texture");
    
    mFinished++;
    
    return DR_OK;
}