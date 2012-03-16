#include "RenderPlanet.h"
#include "ShaderManager.h"
#include "GlobalRenderer.h"
#include "DRTextureManager.h"


RenderPlanet::RenderPlanet(SektorID seed, DRString texturePath, const PlanetNoiseParameter* planetNoiseParameter)
: RenderSektor(), mTextureRenderer(), mTexture(),
  mPreviewTextur(), mInitalized(0)
{
    int size = GlobalRenderer::Instance().getTextureRenderMaxResolution()/2;
    init(seed, DRVector3(0.0f), 1.0f, DRMatrix::identity(), "noise.vert", "noise.frag", 
         size, texturePath, planetNoiseParameter);
}

RenderPlanet::RenderPlanet(SektorID seed, DRVector3 translate,
                           float patchScaling, const DRMatrix& rotation, DRString texturePath,
                           const PlanetNoiseParameter* planetNoiseParameter)
: RenderSektor(), mTextureRenderer(), mTexture(), 
  mPreviewTextur(), mInitalized(0)
{
    int size = GlobalRenderer::Instance().getTextureRenderMaxResolution();
	//printf("[RenderPlanet::RenderPlanet] size from GlobalRenderer: %d\n", size);
    init(seed, translate, patchScaling, rotation, "subPlanetNoise.vert", "noise.frag", 
         size, texturePath, planetNoiseParameter);
}

DRReturn RenderPlanet::init(SektorID seed, DRVector3 translate,
              float patchScaling, const DRMatrix& rotation, 
              const char* vertexShader, const char* fragmentShader, int textureSize, DRString texturePath,
              const PlanetNoiseParameter* planetNoiseParameter)
{
	GlobalRenderer& gb = GlobalRenderer::Instance();
	mShader = ShaderManager::Instance().getShader("simple.vert", "simple.frag");

	int stepSizei = gb.getTextureRenderStepSize();
	float stepSize = static_cast<float>(stepSizei);

	//clear texture
	size_t bufferSize = stepSizei*stepSizei*4;
	char* buffer = new char[bufferSize];
	memset(buffer, 0, bufferSize);

	mPreviewTextur = TexturePtr(new Texture(stepSizei, stepSizei, GL_UNSIGNED_BYTE, 4, buffer, bufferSize));
	DR_SAVE_DELETE_ARRAY(buffer);
	buffer = NULL;
	mPreviewTextur->setWrappingMode(TEXTURE_WRAPPING_CLAMP_TO_EDGE);
    mTexturePath = texturePath;

	mTextureRenderer = RenderInStepsToTexturePtr(new RenderNoisePlanetToTexture(vertexShader, fragmentShader, planetNoiseParameter));
	dynamic_cast<RenderNoisePlanetToTexture*>(mTextureRenderer.getResourcePtrHolder()->mResource)->init(stepSize, translate, patchScaling, mPreviewTextur, rotation);    
	GlobalRenderer::Instance().addRenderTask(mTextureRenderer, true);
		
    if(DRIsFileExist(getPathAndFilename().data()))
    {
        mTexture = TexturePtr(new Texture(getPathAndFilename().data()));
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
		mTexture = TexturePtr(new Texture(textureSize, textureSize, GL_UNSIGNED_BYTE, 4));

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
	ShaderManager::Instance().releaseShader(mShader);
    if(mTextureRenderer.getResourcePtrHolder() && !mTextureRenderer->isFinished())
        GlobalRenderer::Instance().removeRenderTask(mTextureRenderer);
    //DR_SAVE_DELETE(mTextureRenderer);
  	//DR_SAVE_DELETE(mTexture);
	//DR_SAVE_DELETE(mPreviewTextur);
}
DRReturn RenderPlanet::generateAndBindTexture()
{
    if(mTextureRenderer.getResourcePtrHolder() && mTextureRenderer->isFinished())
    {
		//finish rendering preview texture
        if(!mInitalized)
        {
			mTextureRenderer->reinit(mTexture);
			GlobalRenderer::Instance().addRenderTask(mTextureRenderer);
			//printf("[RenderPlanet::generateAndBindTexture]  reinint\n");
			mInitalized++;
		}
		//finish rendering texture
		else if(1 == mInitalized)
		{
			mPreviewTextur.release();
			if(mTexturePath.size() > 0)
			{
				//DRLog.writeToLog("Dateiname fur Textur: %s", filename.data());
				//mTextureRenderer->saveImageToFile(getPathAndFilename().data());
				mTexture->getPixelsToSave(getPathAndFilename().data());
				DRTextureManager::Instance().addAsynchronTextureSaveTask(mTexture);
			}
			mTextureRenderer.release();
		}
	}
	if(3 == mInitalized && mTexture->isLoadingFinished() && mTextureRenderer->isFinished())
	{
		mPreviewTextur.release();
        mTextureRenderer.release();
		mInitalized++;
    }    
	if(mTexture->isLoadingError())
	{
		int size = GlobalRenderer::Instance().getTextureRenderMaxResolution();
		mTexture = TexturePtr(new Texture(size, size, GL_UNSIGNED_BYTE, 4));
		mInitalized = 0;
	}
    glEnable(GL_TEXTURE_2D);
    if(mPreviewTextur.getResourcePtrHolder())
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

    //korrektur damit gluSphere textur die selbe ist wie bei childs, daher wird die Kugel gedreht
    DRMatrix currentModelview = mShader->getUniformMatrix("modelview");
    currentModelview = DRMatrix::rotationY(PI/2.0f) * DRMatrix::rotationZ(-PI/2.0f) * currentModelview;
    mShader->setUniformMatrix("modelview", currentModelview);

    gluSphere(GlobalRenderer::Instance().getQuadric(), 1.0f, quadricDetails*2, quadricDetails);
        
    if(DRGrafikError("[RenderPlanet::render]")) return DR_ERROR;
    return DR_OK;
}

