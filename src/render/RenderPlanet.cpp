#include "RenderPlanet.h"
#include "ShaderManager.h"
#include "GlobalRenderer.h"
#include "DRTextureManager.h"
#include "PlanetSektor.h"
#include "RenderTextureToTexture.h"


RenderPlanet::RenderPlanet(SektorID seed, DRString texturePath, const PlanetNoiseParameter* planetNoiseParameter)
: RenderSektor(), mTextureRenderer(), mSeaLevelInMetres(0.0f), mTexture(), mInitalized(0), mHeightMap(NULL)
{
    int size = GlobalRenderer::Instance().getTextureRenderMaxResolution()/2;
    ShaderProgramParameter shader[4];
    shader[0] = ShaderProgramParameter("noise.vert", "PlanetContinentalNoise.frag");
    shader[1] = ShaderProgramParameter("noise.vert", "PlanetTerrainTypeNoise.frag");
    shader[2] = ShaderProgramParameter("noise.vert", "PlanetBadlandSands.frag");
    shader[3] = ShaderProgramParameter("noise.vert", "PlanetNoiseFinal.frag");
    init(seed, DRVector3(0.0f), 1.0f, DRMatrix::identity(), shader,
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
    ShaderProgramParameter shader[4];
    shader[0] = ShaderProgramParameter("subPlanetNoise.vert", "PlanetContinentalNoise.frag");
    shader[1] = ShaderProgramParameter("subPlanetNoise.vert", "PlanetTerrainTypeNoise.frag");
    shader[2] = ShaderProgramParameter("subPlanetNoise.vert", "PlanetBadlandSands.frag");
    shader[3] = ShaderProgramParameter("subPlanetNoise.vert", "PlanetNoiseFinal.frag");
    init(seed, translate, patchScaling, rotation, shader,
         size, texturePath, planetNoiseParameter);
}

DRReturn RenderPlanet::init(SektorID seed, DRVector3 translate,
              float patchScaling, const DRMatrix& rotation, 
              ShaderProgramParameter shader[4], int textureSize, DRString texturePath,
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
		mHeightTexture = tx.getTexture(DRVector2i(textureSize), 4);
		if(mHeightTexture.getResourcePtrHolder())
        {
			mHeightTexture->setWrappingMode(TEXTURE_WRAPPING_CLAMP_TO_EDGE);
            mHeightTexture->setFilter(GL_NEAREST, GL_NEAREST);
        }
        if(!mHeightTexture) LOG_ERROR("Zero-Pointer texture", DR_ERROR);
                
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
}
DRReturn RenderPlanet::generateTexture()
{
    if(mTextureRenderer && mTextureRenderer->isFinished())
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
	}
        
    if(DRGrafikError("[RenderPlanet::generateTexture]")) return DR_ERROR;
    return DR_OK;
}

DRReturn RenderPlanet::bindTexture()
{
    glEnable(GL_TEXTURE_2D);
    if(this->isFinishLoading() && mTexture)
        mTexture->bind();
    else
        mHeightTexture->bind();
    
    mShader->setUniform1f("SEA_LEVEL_IN_METRES", mSeaLevelInMetres);    
    
    if(DRGrafikError("[RenderPlanet::bindTexture]")) return DR_ERROR;
    return DR_OK;
}
DRReturn RenderPlanet::generateFinalTexture()
{
    mTextureRenderer.release();            
    RenderTextureToTexture* textureRenderer = new RenderTextureToTexture;
    mTextureRenderer = RenderToTexturePtr(textureRenderer);
    textureRenderer->init(mTexture, mHeightTexture, ShaderManager::Instance().getShader("simpleRenderToTexture.vert", "gradientColor.frag"));
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

