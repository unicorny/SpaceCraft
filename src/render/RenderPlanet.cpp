#include "RenderPlanet.h"
#include "ShaderManager.h"
#include "GlobalRenderer.h"
#include "DRTextureManager.h"


RenderPlanet::RenderPlanet(SektorID seed, DRString texturePath, const PlanetNoiseParameter* planetNoiseParameter)
: RenderSektor(), mTextureRenderer(), mTexture(),
  mPreviewTextur(), mInitalized(0), mUsingParentTexture(false)
{
    int size = GlobalRenderer::Instance().getTextureRenderMaxResolution()/2;
    init(seed, DRVector3(0.0f), 1.0f, DRMatrix::identity(), "noise.vert", "noise.frag", 
         size, texturePath, planetNoiseParameter, DRTexturePtr());
}

RenderPlanet::RenderPlanet(SektorID seed, DRVector3 translate,
                           float patchScaling, const DRMatrix& rotation, DRString texturePath,
                           const PlanetNoiseParameter* planetNoiseParameter, DRTexturePtr parentTexture)
: RenderSektor(), mTextureRenderer(), mTexture(), 
  mPreviewTextur(), mInitalized(0), mUsingParentTexture(false)
{
    int size = GlobalRenderer::Instance().getTextureRenderMaxResolution();
	//printf("[RenderPlanet::RenderPlanet] size from GlobalRenderer: %d\n", size);
    init(seed, translate, patchScaling, rotation, "subPlanetNoise.vert", "noise.frag", 
         size, texturePath, planetNoiseParameter, parentTexture);
}

DRReturn RenderPlanet::init(SektorID seed, DRVector3 translate,
              float patchScaling, const DRMatrix& rotation, 
              const char* vertexShader, const char* fragmentShader, int textureSize, DRString texturePath,
              const PlanetNoiseParameter* planetNoiseParameter, DRTexturePtr parentTexture)
{
    GlobalRenderer& gb = GlobalRenderer::Instance();
    DRTextureManager& tx = DRTextureManager::Instance();
	mShader = ShaderManager::Instance().getShader("simple.vert", "simple.frag");

	int stepSize = gb.getTextureRenderStepSize();
	//float stepSize = static_cast<float>(stepSizei);

    if(parentTexture.getResourcePtrHolder())
    {
        mPreviewTextur = parentTexture;
        mUsingParentTexture = true;
    }
    else
    {
        mPreviewTextur = tx.getTexture(DRVector2i(stepSize), 4);
    
        if(mPreviewTextur.getResourcePtrHolder())
            mPreviewTextur->setWrappingMode(TEXTURE_WRAPPING_CLAMP_TO_EDGE);
        mTexturePath = texturePath;

        mTextureRenderer = RenderInStepsToTexturePtr(new RenderNoisePlanetToTexture(vertexShader, fragmentShader, planetNoiseParameter));
        getRenderNoisePlanetToTexture()->init(stepSize, translate, patchScaling, mPreviewTextur, rotation);    
        GlobalRenderer::Instance().addRenderTask(mTextureRenderer, true);
    }
		
    if(DRIsFileExist(getPathAndFilename().data()))
    {
        mTexture = tx.getTexture(getPathAndFilename().data(), true);
            //TexturePtr(new Texture(getPathAndFilename().data()));
		//mTexture->loadFromFile();
		//mTexture->pixelsCopyToRenderer();
//		DRTextureManager::Instance().addAsynchronTextureLoadTask(mTexture);
		mInitalized = 3;
    }
    else
    {     
        //mTextureID = DRTextureManager::Instance().getGLTextureMemory(textureSize, textureSize,
          //                                                           GL_UNSIGNED_BYTE, 4);    
//        mPreviewTextureID = DRTextureManager::Instance().getGLTextureMemory(stepSizei, stepSizei,
  //                                                                   GL_UNSIGNED_BYTE, 4);    		
		mTexture = tx.getTexture(DRVector2i(textureSize), 4);
		if(mTexture.getResourcePtrHolder())
			mTexture->setWrappingMode(TEXTURE_WRAPPING_CLAMP_TO_EDGE);
            //TexturePtr(new Texture(textureSize, textureSize, GL_UNSIGNED_BYTE, 4));
        if(mUsingParentTexture)
        {
            mTextureRenderer = RenderInStepsToTexturePtr(new RenderNoisePlanetToTexture(vertexShader, fragmentShader, planetNoiseParameter));
            getRenderNoisePlanetToTexture()->init(stepSize, translate, patchScaling, mTexture, rotation);    
            GlobalRenderer::Instance().addRenderTask(mTextureRenderer, false);
            mInitalized = 1;
        }
        //mTextureRenderer->init(stepSize, theta, 1.0f-cameraDistance, mPreviewTextur, rotation);    
    }
    
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
            mPreviewTextur->setFinishRender();
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
                DRTextureManager::Instance().saveTexture(mTexture, getPathAndFilename().data(),
                    GlobalRenderer::Instance().getTextureRenderStepSize()*
                    GlobalRenderer::Instance().getTextureRenderStepSize());
			}
			mTextureRenderer.release();
            mTexture->setFinishRender();
		}
	}
	if(3 == mInitalized && mTexture->isLoadingFinished() && mTextureRenderer->isFinished())
	{
		mPreviewTextur.release();
        mTextureRenderer.release();
        mTexture->setFilter(GL_LINEAR, GL_LINEAR);
        mTexture->setWrappingMode(TEXTURE_WRAPPING_CLAMP_TO_EDGE);
		mInitalized++;
    }    
	if(mTexture->isLoadingError())
	{
		int size = GlobalRenderer::Instance().getTextureRenderMaxResolution();
		mTexture = DRTextureManager::Instance().getTexture(DRVector2i(size), 4);
            //TexturePtr(new Texture(size, size, GL_UNSIGNED_BYTE, 4));
		mInitalized = 0;
	}
    glEnable(GL_TEXTURE_2D);
    GLint textureLoaded = 0; //render not
    if(mPreviewTextur.getResourcePtrHolder())
    {
        mPreviewTextur->bind();
        if(mPreviewTextur->isLoadingFinished()) 
            textureLoaded = 1;
        if(!mUsingParentTexture)
            mShader->setUniform2fv("textureCoordsParam", DRVector2(0.0f));
    }
    else
    {
        mTexture->bind();
        textureLoaded = 1;
        // nur für debug auskommentiert
        mShader->setUniform2fv("textureCoordsParam", DRVector2(0.0f));
    }
    mShader->setUniform1i("textureLoaded", textureLoaded);
    
    
    if(DRGrafikError("[RenderPlanet::generateAndBindTexture]")) return DR_ERROR;
    return DR_OK;
}

RenderNoisePlanetToTexture* RenderPlanet::getRenderNoisePlanetToTexture()
{
	if(!mTextureRenderer.getResourcePtrHolder()) return NULL;
    return dynamic_cast<RenderNoisePlanetToTexture*>(mTextureRenderer.getResourcePtrHolder()->mResource);
}

DRReturn RenderPlanet::render(float fTime, Camera* cam)
{    
    uint quadricDetails = 32;
    if(mDetailLevel > 9) quadricDetails = 256; // 10
    else if(mDetailLevel > 6) quadricDetails = 128; //7
    else if(mDetailLevel > 2) quadricDetails = 64; // 3
    else if(mDetailLevel > 0) quadricDetails = 32; // 1
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

