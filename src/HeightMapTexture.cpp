#include "HeightMapTexture.h"

HeightMapTexture::HeightMapTexture(int stepSize)
: mPixelHeightData(NULL), mLoadStepSize(stepSize), mSize(0), mState(0), mPixelCopyMutex(NULL),
  mMaxHeight(0.0f), mMaxGradient(0.0f)
{
    mPixelCopyMutex = SDL_CreateMutex(); LOG_WARNING_SDL();
        
}

HeightMapTexture::~HeightMapTexture()
{
    DR_SAVE_DELETE_ARRAY(mPixelHeightData);
    SDL_LockMutex(mPixelCopyMutex); SDL_UnlockMutex(mPixelCopyMutex); 
    SDL_DestroyMutex(mPixelCopyMutex); LOG_WARNING_SDL();    
}

void HeightMapTexture::load(DRTexturePtr source)
{
    new HeightMapLoader(source, mLoadStepSize, this);
}

void HeightMapTexture::copyPixelData(u8* data, DRVector2i size)
{
    if(!mPixelHeightData || mSize != size) 
    {
        DR_SAVE_DELETE_ARRAY(mPixelHeightData);
        mSize = size;
        mMaxGradient = -10.0f;
        mMaxHeight = -10.0f;
        mPixelHeightData = new float[size.x*size.y];
    }
    if(sizeof(float) != sizeof(u8)*4) LOG_ERROR_VOID("datatypes not like expected");
    //memcpy(mPixelHeightData, data, 4*size.x*size.y*sizeof(u8));
    
    for(uint y = 0; y < size.y; y++)
    {
        for(uint x = 0; x < size.x; x++)
        {
            uint i = x+y*size.x;
            uint ix = x-1+y*size.x;
            uint iy = x+(y-1)*size.x;
            float red = static_cast<float>(data[i*4+2]);
            float green = static_cast<float>(data[i*4+1]);
            float blue = static_cast<float>(data[i*4]);
            mPixelHeightData[i] = ((red+green*256.0f + (blue*256.0f*256.0f))/(256.0f*256.0f*256.0f))*2.0f-1.0f; 

            if(mPixelHeightData[i] > mMaxHeight)
                mMaxHeight = mPixelHeightData[i];
            if(x > 0)
            {
                float xGradient = fabs(mPixelHeightData[i] - mPixelHeightData[ix]);
                if(xGradient > mMaxGradient)
                    mMaxGradient = xGradient;
            }
            if(y > 0)
            {
                float yGradient = fabs(mPixelHeightData[i] - mPixelHeightData[iy]);
                if(yGradient > mMaxGradient)
                    mMaxGradient = yGradient;
            }
            
        }
    }
    DREngineLog.writeToLog("[HeightMapTexture::copyPixelData]: maxHeight: %f, maxGradient: %f", mMaxHeight, mMaxGradient);
    
    SDL_LockMutex(mPixelCopyMutex); LOG_WARNING_SDL();    
    mState++;
    SDL_UnlockMutex(mPixelCopyMutex); LOG_WARNING_SDL();    
}

// ----------------------------------------------------------------------------------

HeightMapLoader::HeightMapLoader(DRTexturePtr source, int stepSize, HeightMapTexture* parent)
: DRSaveTexture("", source->getTextureBuffer(), stepSize), mParent(parent)
{
    DRTextureManager::Instance().saveTexture(source, this);    
}

HeightMapLoader::~HeightMapLoader()
{
    
}

DRReturn HeightMapLoader::saveImage()
{
	if(mSavingState < 2) LOG_ERROR("Image data not ready!", DR_ERROR);

	if(mSize.x <= 0 || mSize.y <= 0) LOG_ERROR("error getSize return zero", DR_ERROR);
	if(mSavingBuffer)
	{
		mImage->setPixel(mSavingBuffer);		
        mParent->copyPixelData(mSavingBuffer, mSize);
		DR_SAVE_DELETE_ARRAY(mSavingBuffer);
	}

	DRIImage::deleteImage(mImage);
	mImage = NULL;
		
	mSavingState = 3;
	//printf("image saved\n");
    
    return DR_OK;
}
