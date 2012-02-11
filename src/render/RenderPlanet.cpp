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
: RenderSektor(), mTextureRenderer(NULL), mTextureID(0), mPreviewTextureID(0), mInitalized(0)
{
    int size = GlobalRenderer::Instance().getTextureRenderMaxResolution()/2;
    init(seed, PI/2.0f, 1.0f, DRMatrix::identity(), "noise.vert", "noise.frag", size, texturePath);
}

RenderPlanet::RenderPlanet(SektorID seed, float theta,
                           float cameraDistance, const DRMatrix& rotation, DRString texturePath)
: RenderSektor(), mTextureRenderer(NULL), mTextureID(0), mPreviewTextureID(0), mInitalized(0)
{
    int size = GlobalRenderer::Instance().getTextureRenderMaxResolution();
    init(seed, theta, cameraDistance, rotation, "subPlanetNoise.vert", "subPlanetNoise.frag", size, texturePath);
}

DRReturn RenderPlanet::init(SektorID seed, float theta, 
              float cameraDistance, const DRMatrix& rotation, 
              const char* vertexShader, const char* fragmentShader, int textureSize, DRString texturePath)
{
    mTexturePath = texturePath;
    //DRTextur tex(getPathAndFilename().data());
    if(false/*tex.isLoaded()*/)
    {
      //  tex.bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //mTextureID = tex.removeTexturID();
        //tex.unload(true);
    }
    else
    {
    
        GlobalRenderer& gb = GlobalRenderer::Instance();

        int stepSizei = gb.getTextureRenderStepSize();
        float stepSize = static_cast<float>(stepSizei);

        mTextureRenderer = new RenderNoisePlanetToTexture(vertexShader, fragmentShader);
        mTextureID = DRTextureManager::Instance().getGLTextureMemory(textureSize, textureSize,
                                                                     GL_UNSIGNED_BYTE, 4);    
        mPreviewTextureID = DRTextureManager::Instance().getGLTextureMemory(stepSizei, stepSizei,
                                                                     GL_UNSIGNED_BYTE, 4);    
        //clear texture
        size_t bufferSize = textureSize*textureSize*4;
        char* buffer = new char[bufferSize];

        for(int i = 0; i < 2; i++)
        {
            memset(buffer, 0, bufferSize);
            if(!i)
            {
                glBindTexture(GL_TEXTURE_2D, mTextureID);
            }
            else if(i == 1)
            {
                glBindTexture(GL_TEXTURE_2D, mPreviewTextureID);
                textureSize = stepSizei;
            }
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 
                textureSize, textureSize,
                GL_RGBA, GL_UNSIGNED_BYTE, buffer);
        }
        DR_SAVE_DELETE_ARRAY(buffer);

        mTextureRenderer->init(stepSize, theta, 1.0f-cameraDistance, mPreviewTextureID, rotation);    
    }
    
    return DR_OK;
}

DRString RenderPlanet::getPathAndFilename()
{
    return DRString(mTexturePath+DRString("planet.png"));
}

RenderPlanet::~RenderPlanet()
{
    DR_SAVE_DELETE(mTextureRenderer);
    if(mTextureID)
    {
        DRTextureManager::Instance().freeTexture(mTextureID);
        mTextureID = 0;
    }
}
DRReturn RenderPlanet::generateAndBindTexture()
{
    if(!mInitalized && mTextureRenderer)
    {
        GlobalRenderer::Instance().addRenderTask(mTextureRenderer, true);
        mInitalized = 1;        
    }
    
    if(mTextureRenderer && mTextureRenderer->isFinished())
    {
        if(mInitalized == 1)
        {
            mTextureRenderer->reinit(mTextureID);
            GlobalRenderer::Instance().addRenderTask(mTextureRenderer);
        }
        else
        {
            DRTextureManager::Instance().freeTexture(mPreviewTextureID);
            mPreviewTextureID = 0;            
            if(mTexturePath.size() > 0)
            {
                //DRLog.writeToLog("Dateiname fur Textur: %s", filename.data());
                mTextureRenderer->saveImageToFile(getPathAndFilename().data());
            }
            DR_SAVE_DELETE(mTextureRenderer);
        }
        mInitalized++;
    }    
    glEnable(GL_TEXTURE_2D);
    if(mPreviewTextureID)
        glBindTexture(GL_TEXTURE_2D, mPreviewTextureID);
    else
        glBindTexture(GL_TEXTURE_2D, mTextureID);
    
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

