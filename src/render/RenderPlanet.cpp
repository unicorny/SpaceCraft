#include "RenderPlanet.h"
#include "ShaderManager.h"
#include "GlobalRenderer.h"
#include "DRTextureManager.h"
#include "PlanetSektor.h"


RenderPlanet::RenderPlanet(SektorID seed, DRString texturePath, const PlanetNoiseParameter* planetNoiseParameter)
: RenderSektor(), mTextureRenderer(), mSeaLevelInMetres(0.0f), mTexture(), mInitalized(0), mHeightMap(NULL)
{
    int size = GlobalRenderer::Instance().getTextureRenderMaxResolution()/2;
    init(seed, DRVector3(0.0f), 1.0f, DRMatrix::identity(), "noise.vert", "noise.frag", 
         size, texturePath, planetNoiseParameter);
}

RenderPlanet::RenderPlanet(SektorID seed, DRVector3 translate,
                           float patchScaling, const DRMatrix& rotation, DRString texturePath,
                           const PlanetNoiseParameter* planetNoiseParameter)
: RenderSektor(), mTextureRenderer(), mSeaLevelInMetres(0.0f), mTexture(), 
  mInitalized(0),mHeightMap(NULL)
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
    DRTextureManager& tx = DRTextureManager::Instance();
	//mShader = ShaderManager::Instance().getShader("simple.vert", "simple.frag");
    mShader = ShaderManager::Instance().getShader("simple.vert", "simpleUnpack.frag");
    mSeaLevelInMetres = planetNoiseParameter->seaLevelInMetres;
    int textureStepSize = gb.getTextureRenderStepSize();
    mHeightMap = new HeightMapTexture(textureStepSize*textureStepSize*16);

	int stepSize = gb.getTextureRenderStepSize();
	//float stepSize = static_cast<float>(stepSizei);

    mTexturePath = texturePath;    
        
    if(DRIsFileExist(getPathAndFilename().data()))
    {
        mTexture = tx.getTexture(getPathAndFilename().data(), true, GL_NEAREST, GL_NEAREST);
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
        {
			mTexture->setWrappingMode(TEXTURE_WRAPPING_CLAMP_TO_EDGE);
            mTexture->setFilter(GL_NEAREST, GL_NEAREST);
        }
        if(!mTexture) LOG_ERROR("Zero-Pointer texture", DR_ERROR);
        
        mTextureRenderer = RenderInStepsToTexturePtr(new RenderNoisePlanetToTexture(vertexShader, fragmentShader, planetNoiseParameter));
        mTextureRenderer->setFilenameToSave(getPathAndFilename());
        getRenderNoisePlanetToTexture()->init(stepSize, translate, patchScaling, mTexture, rotation);    
        
        GlobalRenderer::Instance().addRenderTask(mTextureRenderer);
        mInitalized = 1;
        
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
    if(mTextureRenderer && mTextureRenderer->isFinished())
    {		
		//finish rendering texture
		if(1 == mInitalized)
		{
            mHeightMap->load(mTexture);
			mTextureRenderer.release();
            mTexture->setFinishRender();
            mInitalized++;
		}
	}
	if(3 == mInitalized && mTexture->isLoadingFinished())// && mTextureRenderer->isFinished())
	{
     //   mTextureRenderer.release();
        mTexture->setFilter(GL_NEAREST, GL_NEAREST);
        mTexture->setWrappingMode(TEXTURE_WRAPPING_CLAMP_TO_EDGE);
		mInitalized++;
        mHeightMap->load(mTexture);
    }    
	if(mTexture && mTexture->isLoadingError())
	{
		int size = GlobalRenderer::Instance().getTextureRenderMaxResolution();
		mTexture = DRTextureManager::Instance().getTexture(DRVector2i(size), 4);
            //TexturePtr(new Texture(size, size, GL_UNSIGNED_BYTE, 4));
		mInitalized = -1;
	}
    glEnable(GL_TEXTURE_2D);
    if(mTexture)
        mTexture->bind();
    
    mShader->setUniform1f("SEA_LEVEL_IN_METRES", mSeaLevelInMetres);    
    
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

