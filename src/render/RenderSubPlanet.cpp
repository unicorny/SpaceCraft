#include "RenderSubPlanet.h"
#include "DRGeometrieManager.h"
#include "ShaderManager.h"

RenderSubPlanet::RenderSubPlanet(DRVector3 translate, float patchScaling, const DRMatrix& rotation, DRString texturePath, const PlanetNoiseParameter* planetNoiseParameter)
: RenderPlanet(translate, patchScaling, rotation, texturePath, planetNoiseParameter), mGeometrieGrid(NULL)
{
    mGeometrieGrid = DRGeometrieManager::Instance().getGrid(100, GEO_FULL, GEO_VERTEX_QUADS);
    mShader = ShaderManager::Instance().getShaderProgram("sphere.vert", "sphere.frag");
}

RenderSubPlanet::RenderSubPlanet(DRTexturePtr parentTexture)
: RenderPlanet(), mParentTexture(parentTexture)
{
    mGeometrieGrid = DRGeometrieManager::Instance().getGrid(200, GEO_FULL, GEO_VERTEX_QUADS);
    mShader = ShaderManager::Instance().getShaderProgram("sphere.vert", "sphere.frag");
}

RenderSubPlanet::~RenderSubPlanet()
{
    //DR_SAVE_DELETE(mGeometrieGrid);
}

DRReturn RenderSubPlanet::render(float fTime, Camera* cam)
{   
    if(!isFinishLoading())
    {
        glEnable(GL_TEXTURE_2D);
        if(mParentTexture)
            mParentTexture->bind();
        else//*/
            glDisable(GL_TEXTURE_2D);
    }
    else
    {
        bindTexture();
    }
    glDisable(GL_CULL_FACE);
    if(mGeometrieGrid->render()) 
        LOG_ERROR("Fehler bei Geometrie Grid", DR_ERROR);
    
    if(DRGrafikError("[RenderSubPlanet::render]")) return DR_ERROR;
    return DR_OK;
}


