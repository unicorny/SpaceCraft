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
RenderPlanet::RenderPlanet(SektorID seed)
: RenderSektor(), mTextureRenderer(NULL), mTextureID(0)
{
    GlobalRenderer& gb = GlobalRenderer::Instance();
    
    ShaderProgram* shader = ShaderManager::Instance().getShader("noise.frag", "noise.vert");
    DRVector2 textureSize = gb.getTextureRenderMaxResolution();
    mTextureRenderer = new RenderNoisePlanetToTexture(shader, textureSize);
    mTextureID = DRTextureManager::Instance().getGLTextureMemory(static_cast<GLuint>(textureSize.x),
                                                                 static_cast<GLuint>(textureSize.y),
                                                                 GL_FLOAT, 4);    
    float stepSize = static_cast<float>(gb.getTextureRenderStepSize());
    mTextureRenderer->init(stepSize, PI/2.0f, 0.0f, mTextureID);    
    gb.addRenderTask(mTextureRenderer);
}

RenderPlanet::~RenderPlanet()
{
    ShaderManager::Instance().releaseShader("noise.frag", "noise.vert");
    DR_SAVE_DELETE(mTextureRenderer);
    if(mTextureID)
    {
        DRTextureManager::Instance().freeTexture(mTextureID);
        mTextureID = 0;
    }
}

DRReturn RenderPlanet::render(float fTime, Camera* cam)
{
    uint quadricDetails = 32;
    if(mDetailLevel > 9) quadricDetails = 256; // 10
    else if(mDetailLevel > 6) quadricDetails = 128; //7
    else if(mDetailLevel > 2) quadricDetails = 64; // 3
    else if(mDetailLevel > 0.0f)  quadricDetails = 32; // 1
    else quadricDetails = 8;
    
    if(mTextureRenderer && mTextureRenderer->isFinished())
    {
        DR_SAVE_DELETE(mTextureRenderer);
    }    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mTextureID);
    
    gluSphere(GlobalRenderer::Instance().getQuadric(), 1.0f, quadricDetails*2, quadricDetails);
    
    if(DRGrafikError("[RenderPlanet::render]")) return DR_ERROR;
    return DR_OK;
}

