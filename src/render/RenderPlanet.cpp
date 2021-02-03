#include "RenderPlanet.h"
#include "ShaderManager.h"
#include "GlobalRenderer.h"
#include "DRTextureManager.h"
#include "PlanetSektor.h"
#include "RenderTextureToTexture.h"

DRString RenderPlanet::mFragmentShader[] = {DRString("PlanetContinentalNoise.frag"),
                                          DRString("PlanetTerrainTypeNoise.frag"),
                                          DRString("PlanetBadlandSands.frag"),
                                          DRString("PlanetWithoutRiver.frag"),
                                          DRString("PlanetNoiseFinal.frag")};

RenderPlanet::RenderPlanet(DRString texturePath, const PlanetNoiseParameter* planetNoiseParameter)
: RenderSektor(), mTextureRenderer(), mSeaLevelInMetres(0.0f), mTexture(),
  mInitalized(0), mHeightMap(NULL), mVertexShader("noise.vert")
{
    int size = GlobalRenderer::Instance().getTextureRenderMaxResolution()/2;
    init(DRVector3(0.0f), 1.0f, DRMatrix::identity(),
         size, texturePath, planetNoiseParameter);
    mShader = ShaderManager::Instance().getShaderProgram("simple.vert", "simpleUnpack.frag");
}

RenderPlanet::RenderPlanet()
: RenderSektor(), mTextureRenderer(), mSeaLevelInMetres(0.0f), mTexture(),
  mInitalized(0), mHeightMap(NULL), mVertexShader("subPlanetNoise.vert")
{
    
}

RenderPlanet::RenderPlanet(DRVector3 translate,
                           float patchScaling, const DRMatrix& rotation, DRString texturePath,
                           const PlanetNoiseParameter* planetNoiseParameter)
: RenderSektor(), mTextureRenderer(), mSeaLevelInMetres(0.0f), mTexture(), 
  mInitalized(0), mHeightMap(NULL), mVertexShader("subPlanetNoise.vert")
{
    int size = GlobalRenderer::Instance().getTextureRenderMaxResolution();
	//printf("[RenderPlanet::RenderPlanet] size from GlobalRenderer: %d\n", size);
    init(translate, patchScaling, rotation,
         size, texturePath, planetNoiseParameter);
}

DRReturn RenderPlanet::init(DRVector3 translate,
              float patchScaling, const DRMatrix& rotation, 
              int textureSize, DRString texturePath,
              const PlanetNoiseParameter* planetNoiseParameter)
{
    if(mInitalized) 
    {
        LOG_WARNING("already initalized");
        return DR_OK;
    }
    GlobalRenderer& gb = GlobalRenderer::Instance();
    DRTextureManager& tx = DRTextureManager::Instance();
	//mShader = ShaderManager::Instance().getShader("simple.vert", "simple.frag");
    
    mSeaLevelInMetres = planetNoiseParameter->seaLevelInMetres;
    int textureStepSize = gb.getTextureRenderStepSize();
    mHeightMap = new HeightMapTexture(textureStepSize*textureStepSize*16);

	int stepSize = gb.getTextureRenderStepSize();
	//float stepSize = static_cast<float>(stepSizei);

    mTexturePath = texturePath;  
	if (textureSize <= 0) {
		LOG_ERROR("texture size is zero", DR_ERROR);
	}
    mTexture = tx.getTexture(DRVector2i(textureSize), 4);    
    mTexture->setWrappingMode(TEXTURE_WRAPPING_CLAMP_TO_EDGE);
    mTexture->setFilter(GL_NEAREST, GL_NEAREST);
    
    if(DRIsFileExist(getPathAndFilename().data()))
    {
        mHeightTexture = tx.getTexture(getPathAndFilename().data(), true, GL_NEAREST, GL_NEAREST);
		mInitalized = 2;
    }
    else
    {     
        //mTextureID = DRTextureManager::Instance().getGLTextureMemory(textureSize, textureSize,
          //                                                           GL_UNSIGNED_BYTE, 4);    
//        mPreviewTextureID = DRTextureManager::Instance().getGLTextureMemory(stepSizei, stepSizei,
  //                                                                   GL_UNSIGNED_BYTE, 4);    		
		if (textureSize <= 0) {
			LOG_ERROR("texture size is zero", DR_ERROR);
		}
		mHeightTexture = tx.getTexture(DRVector2i(textureSize), 4);
		if(mHeightTexture.getResourcePtrHolder())
        {
			mHeightTexture->setWrappingMode(TEXTURE_WRAPPING_CLAMP_TO_EDGE);
            mHeightTexture->setFilter(GL_NEAREST, GL_NEAREST);
        }
        if(!mHeightTexture) LOG_ERROR("Zero-Pointer texture", DR_ERROR);
                
        ShaderProgramParameter shader[5];
        for(int i = 0; i < 5; i++)
            shader[i] = ShaderProgramParameter(mVertexShader.data(), (mFragmentShader[i]).data());
        mTextureRenderer = RenderToTexturePtr(new RenderNoisePlanetToTexture(shader, planetNoiseParameter));
        mTextureRenderer->setFilenameToSave(getPathAndFilename());
        getRenderNoisePlanetToTexture()->init(stepSize, translate, patchScaling, mHeightTexture, rotation);    
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
//    ShaderManager::Instance().releaseShader(mShader);
    if(mTextureRenderer.getResourcePtrHolder() && !mTextureRenderer->isFinished())
        GlobalRenderer::Instance().removeRenderTask(mTextureRenderer);
    //DR_SAVE_DELETE(mTextureRenderer);
  	//DR_SAVE_DELETE(mTexture);
	//DR_SAVE_DELETE(mPreviewTextur);
    DR_SAVE_DELETE(mHeightMap);
}
DRReturn RenderPlanet::generateTexture()
{
    if(mTextureRenderer.getResourcePtrHolder() && mTextureRenderer->isFinished())
    {		
		//finish rendering texture, render after color
		if(1 == mInitalized)
		{
            //mHeightMap->load(mTexture);
            mInitalized++;
            mHeightMap->load(mHeightTexture);
			generateFinalTexture();
		}
        else if(3 == mInitalized)
        {
            mTextureRenderer.release();
            mInitalized++;
            mTexture->setFinishRender();            
            if(!mTexture->isLoadingFinished())
                LOG_ERROR("texture wasn't empty", DR_ERROR);
        }
	}
	if(2 == mInitalized && mHeightTexture->isLoadingFinished())// && mTextureRenderer->isFinished())
	{
     //   mTextureRenderer.release();
        mHeightTexture->setFilter(GL_NEAREST, GL_NEAREST);
        mHeightTexture->setWrappingMode(TEXTURE_WRAPPING_CLAMP_TO_EDGE);
        mHeightMap->load(mHeightTexture);        
        generateFinalTexture();
    }    
	if(mHeightTexture && mHeightTexture->isLoadingError())
	{
        // remove corrupt texture file
        remove(mHeightTexture->getTextureFilename().data());
        mInitalized = -1;
        LOG_WARNING("corrupt texture file");
	}
        
    if(DRGrafikError("[RenderPlanet::generateTexture]")) return DR_ERROR;
    return DR_OK;
}

DRReturn RenderPlanet::bindTexture()
{
    glEnable(GL_TEXTURE_2D);
    if(this->isFinishLoading() && mTexture)
        mTexture->bind();
    //else if(mHeightTexture)
    //    mHeightTexture->bind();
    else
        glDisable(GL_TEXTURE_2D);
    
    mShader->setUniform1f("SEA_LEVEL_IN_METRES", mSeaLevelInMetres);    
    
    if(DRGrafikError("[RenderPlanet::bindTexture]")) return DR_ERROR;
    return DR_OK;
}
DRReturn RenderPlanet::generateFinalTexture()
{
    mTextureRenderer.release();            
    RenderTextureToTexture* textureRenderer = new RenderTextureToTexture;
    mTextureRenderer = RenderToTexturePtr(textureRenderer);
    textureRenderer->init(mTexture, mHeightTexture, ShaderManager::Instance().getShaderProgram("simpleRenderToTexture.vert", "gradientColor.frag"));
    textureRenderer->setFilenameToSave(DRString(mTexturePath+DRString("planet_color.png")));
    GlobalRenderer::Instance().addRenderTask(mTextureRenderer, true);
    //mTexture->setFinishRender();
    mInitalized++;
    
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
    
    bindTexture();

    //korrektur damit gluSphere textur die selbe ist wie bei childs, daher wird die Kugel gedreht
    DRMatrix currentModelview = mShader->getUniformMatrix("modelview");
    currentModelview = DRMatrix::rotationY(PI/2.0f) * DRMatrix::rotationZ(-PI/2.0f) * currentModelview;
    mShader->setUniformMatrix("modelview", currentModelview);

    gluSphere(GlobalRenderer::Instance().getQuadric(), 1.0f, quadricDetails*2, quadricDetails);
        
    if(DRGrafikError("[RenderPlanet::render]")) return DR_ERROR;
    return DR_OK;
}

