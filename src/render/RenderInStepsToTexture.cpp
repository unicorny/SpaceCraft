#include "main.h"

RenderInStepsToTexture::RenderInStepsToTexture()
: mTexture(NULL), mStepSize(0), mIndexStepMode(0), mFinished(false)
{
    
}

RenderInStepsToTexture::~RenderInStepsToTexture()
{
    
}

DRReturn RenderInStepsToTexture::init(float stepSize, float clippingBorder[4], Texture* texture)
{
	if(!texture) return DR_ZERO_POINTER;
	Eigen::Vector2i texSize = texture->getResolution();
	if(texSize(0) <= 0 || texSize(1) <= 0)
		LOG_ERROR("textureSize is invalid", DR_ERROR);
	mTextureSize = DRVector2(texSize(0), texSize(1));
        
    mStepSize = stepSize;
    //mTextureSize = textureSize;
    mCursorIndex = mTextureSize/2.0f - stepSize/2.0f;
    mIndexStepMode = 0;
    mCursorMaxCount = 1;
    mCursorCurrentCount = 0;
    mTexture = texture;
    mFinished = false;
    memcpy(mClippingBorder, clippingBorder, sizeof(float)*4);
    
    return DR_OK;
}

DRReturn RenderInStepsToTexture::reinit(Texture* texture)
{
	if(!texture) return DR_ZERO_POINTER;
	Eigen::Vector2i texSize = texture->getResolution();
	if(texSize(0) <= 0 || texSize(1) <= 0)
		LOG_ERROR("textureSize is invalid", DR_ERROR);
	mTextureSize = DRVector2(texSize(0), texSize(1));
    
   // mTextureSize *= textureSizeScalar;
 //   mTextureSize = textureSize;
    mTexture = texture;
    mCursorIndex = mTextureSize/2.0f - mStepSize/2.0f;
    mIndexStepMode = 0;
    mCursorMaxCount = 1;
    mCursorCurrentCount = 0;
    mFinished = false;
    
    return DR_OK;
}

DRReturn RenderInStepsToTexture::step()
{
     if(mCursorIndex > mTextureSize || mCursorIndex < DRVector2(0.0f))
     {
        mFinished = true;
        return DR_OK;
     }
    
    //mFrameBuffer->bindToRender();
    
    //render stuff
    if(mCursorIndex == mTextureSize/2.0f - mStepSize/2.0f)
    {
        glClearColor(0.0, 0.0, 0.0, 0);
        glClear (GL_COLOR_BUFFER_BIT);   
    }
    
    glColor3f(0.0f, 1.0f, 0.0f);    
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
           
    //mCursorIndex / mTextureSize
    DRVector2 clippingSize = DRVector2(mClippingBorder[1]-mClippingBorder[0],
                                       mClippingBorder[3]-mClippingBorder[2]);
    DRVector2 cursor = mCursorIndex / mTextureSize * clippingSize + DRVector2(mClippingBorder[0], mClippingBorder[2]);
    DRVector2 halfStepSize = (DRVector2(mStepSize) / mTextureSize * clippingSize) * 0.5f;
    
    //Reseten der Matrixen
    glViewport(mCursorIndex.x-mStepSize/2.0f, mCursorIndex.y-mStepSize/2.0f, mStepSize, mStepSize);
    
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
DRReturn RenderInStepsToTexture::saveImageToFile(const char* path)
{
    DRIImage* image = DRIImage::newImage();
    u8* buffer = (u8*)malloc(mTextureSize.x*mTextureSize.y*4*sizeof(u8));
	mTexture->bind();
    //glBindTexture(GL_TEXTURE_2D, mTextureID);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, buffer);
    if(DRGrafikError("RenderInStepsToTexture::saveImageToFile")) LOG_ERROR("Fehler bei getting texture Data!", DR_ERROR);
    image->setSize(mTextureSize);
    image->setImageFormat(-1);
    image->setPixel(buffer);
    if(image->saveIntoFile(path))
        LOG_ERROR("fehler bei save", DR_ERROR);
    
    free(buffer);
    delete image;
    return DR_OK;
}