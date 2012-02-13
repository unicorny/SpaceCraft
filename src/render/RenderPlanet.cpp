#include "main.h"


PlanetHeightValues::PlanetHeightValues(GenerateNoisePlanet* noisePlanet)
: mNoisePlanet(noisePlanet)
{
    
}

PlanetHeightValues::~PlanetHeightValues()
{

}

float PlanetHeightValues::getHeightValue(DRVector3& position)
{
    return mNoisePlanet->getValue(position);//*0.00128441521;
}

DRColor PlanetHeightValues::getColorValue(const float height)
{
    noise::utils::Color c = mColor.GetColor(height);///0.00128441521);
    return DRColor(c.red, c.green, c.blue, c.alpha);
}

// ********************************************************************************
// *******************************************************************************
RenderPlanet::RenderPlanet(SektorID seed, DRString texturePath)
: RenderSektor(), mTextureRenderer(NULL), mTexture(NULL),
  mPreviewTextur(NULL), mInitalized(0)
{
    int size = GlobalRenderer::Instance().getTextureRenderMaxResolution()/2;
    init(seed, PI/2.0f, 1.0f, DRMatrix::identity(), "noise.vert", "noise.frag", size, texturePath);
}

RenderPlanet::RenderPlanet(SektorID seed, float theta,
                           float cameraDistance, const DRMatrix& rotation, DRString texturePath)
: RenderSektor(), mTextureRenderer(NULL), mTexture(NULL), 
  mPreviewTextur(NULL), mInitalized(0)
{
    int size = GlobalRenderer::Instance().getTextureRenderMaxResolution();
    init(seed, theta, cameraDistance, rotation, "subPlanetNoise.vert", "subPlanetNoise.frag", size, texturePath);
}

DRReturn RenderPlanet::init(SektorID seed, float theta, 
              float cameraDistance, const DRMatrix& rotation, 
              const char* vertexShader, const char* fragmentShader, int textureSize, DRString texturePath)
{
	GlobalRenderer& gb = GlobalRenderer::Instance();

	int stepSizei = gb.getTextureRenderStepSize();
	float stepSize = static_cast<float>(stepSizei);

	//clear texture
	size_t bufferSize = stepSizei*stepSizei*4;
	char* buffer = new char[bufferSize];
	memset(buffer, 0, bufferSize);

	mPreviewTextur = new Texture(stepSizei, stepSizei, GL_UNSIGNED_BYTE, 4, buffer, bufferSize);
	DR_SAVE_DELETE_ARRAY(buffer);
	buffer = NULL;
	mPreviewTextur->setWrappingMode(TEXTURE_WRAPPING_CLAMP_TO_EDGE);
    mTexturePath = texturePath;

	mTextureRenderer = new RenderNoisePlanetToTexture(vertexShader, fragmentShader);
	mTextureRenderer->init(stepSize, theta, 1.0f-cameraDistance, mPreviewTextur, rotation);    
	GlobalRenderer::Instance().addRenderTask(mTextureRenderer, true);
		
    if(DRIsFileExist(getPathAndFilename().data()))
    {
        mTexture = new Texture(getPathAndFilename().data());
		//mTexture->loadFromFile();
		//mTexture->pixelsCopyToRenderer();
		DRTextureManager::Instance().addAsynchronTextureLoadTask(mTexture);
		mInitalized = 3;
    }
    else
    {     
        //mTextureID = DRTextureManager::Instance().getGLTextureMemory(textureSize, textureSize,
          //                                                           GL_UNSIGNED_BYTE, 4);    
//        mPreviewTextureID = DRTextureManager::Instance().getGLTextureMemory(stepSizei, stepSizei,
  //                                                                   GL_UNSIGNED_BYTE, 4);    		
		mTexture = new Texture(textureSize, textureSize, GL_UNSIGNED_BYTE, 4);

        //mTextureRenderer->init(stepSize, theta, 1.0f-cameraDistance, mPreviewTextur, rotation);    
    }
	mTexture->setWrappingMode(TEXTURE_WRAPPING_CLAMP_TO_EDGE);
    
    return DR_OK;
}

DRString RenderPlanet::getPathAndFilename()
{
    return DRString(mTexturePath+DRString("planet.png"));
}

RenderPlanet::~RenderPlanet()
{
    DR_SAVE_DELETE(mTextureRenderer);
  	DR_SAVE_DELETE(mTexture);
	DR_SAVE_DELETE(mPreviewTextur);
}
DRReturn RenderPlanet::generateAndBindTexture()
{
    if(mTextureRenderer && mTextureRenderer->isFinished())
    {
		//finish rendering preview texture
        if(!mInitalized)
        {
			mTextureRenderer->reinit(mTexture);
			GlobalRenderer::Instance().addRenderTask(mTextureRenderer);
			printf("reinint\n");
			mInitalized++;
		}
		//finish rendering texture
		else if(1 == mInitalized)
		{
			DR_SAVE_DELETE(mPreviewTextur);
			if(mTexturePath.size() > 0)
			{
				//DRLog.writeToLog("Dateiname fur Textur: %s", filename.data());
				//mTextureRenderer->saveImageToFile(getPathAndFilename().data());
				mTexture->getPixelsToSave(getPathAndFilename().data());
				DRTextureManager::Instance().addAsynchronTextureSaveTask(mTexture);
			}
			DR_SAVE_DELETE(mTextureRenderer);
		}
	}
	if(3 == mInitalized && mTexture->isLoadingFinished() && mTextureRenderer->isFinished())
	{
		DR_SAVE_DELETE(mPreviewTextur);
        DR_SAVE_DELETE(mTextureRenderer);
		mInitalized++;
    }    
	if(mTexture->isLoadingError())
	{
		DR_SAVE_DELETE(mTexture);
		int size = GlobalRenderer::Instance().getTextureRenderMaxResolution();
		mTexture = new Texture(size, size, GL_UNSIGNED_BYTE, 4);
		mInitalized = 0;
	}
    glEnable(GL_TEXTURE_2D);
    if(mPreviewTextur)
        mPreviewTextur->bind();
    else
        mTexture->bind();
    
    if(DRGrafikError("[RenderPlanet::generateAndBindTexture]")) return DR_ERROR;
    return DR_OK;
}
DRReturn RenderPlanet::render(float fTime, Camera* cam)
{    
    uint quadricDetails = 32;
    if(mDetailLevel > 9) quadricDetails = 256; // 10
    else if(mDetailLevel > 6) quadricDetails = 128; //7
    else if(mDetailLevel > 2) quadricDetails = 64; // 3
    else if(mDetailLevel > 0.0f)  quadricDetails = 32; // 1
    else quadricDetails = 8;
    
    generateAndBindTexture();
    
    //korrektur damit textur die selbe ist wie bei childs
    glPushMatrix();
    glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    gluSphere(GlobalRenderer::Instance().getQuadric(), 1.0f, quadricDetails*2, quadricDetails);
    
    glPopMatrix();
    
    if(DRGrafikError("[RenderPlanet::render]")) return DR_ERROR;
    return DR_OK;
}

