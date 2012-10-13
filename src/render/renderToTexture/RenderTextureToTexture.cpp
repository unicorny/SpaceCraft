#include "RenderTextureToTexture.h"
#include "DRGeometrieManager.h"

RenderTextureToTexture::RenderTextureToTexture()
: mShader(NULL)
{
    
}

RenderTextureToTexture::~RenderTextureToTexture()
{
    
}

DRReturn RenderTextureToTexture::init(DRTexturePtr targetTexture, DRTexturePtr sourceTexture, ShaderProgram* shader)
{
    if(RenderToTexture::init(targetTexture))
        LOG_ERROR("error by init RenderToTexture", DR_ERROR);
    if(!sourceTexture) LOG_ERROR("Zero-Pointer source texture", DR_ZERO_POINTER);
    if(!shader) LOG_ERROR("Zero-Pointer shader", DR_ZERO_POINTER);

    mSourceTexture = sourceTexture;
    mShader = shader;
    return DR_OK;
}

DRReturn RenderTextureToTexture::renderStuff()
{
    if(!mSourceTexture) LOG_ERROR("source texture Zero-Pointer", DR_ZERO_POINTER);
    if(!mShader) return DR_ZERO_POINTER;
    mSourceTexture->bind();
    mShader->bind();
    mShader->setUniformMatrix("projection", mProjectionMatrix);
    DRGeometrieManager::Instance().getGrid(2, GEO_FULL, GEO_VERTEX_TRIANGLE_STRIP)->render();
    mShader->unbind();
    return DR_OK;
}