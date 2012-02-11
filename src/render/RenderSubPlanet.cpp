#include "main.h"
#include "DRGeometrieManager.h"

RenderSubPlanet::RenderSubPlanet(SektorID seed, float theta, float cameraDistance, const DRMatrix& rotation, DRString texturePath)
: RenderPlanet(seed, theta, cameraDistance, rotation, texturePath), mGeometrieGrid(NULL)
{
        
    //mGeometrieGrid = new DRGeometrieHeightfield(sphericalCenter);
    //mGeometrieGrid->initHeightfield(boxSideEdges, 33, mHeights);
    mGeometrieGrid = DRGeometrieManager::Instance().getGrid(50, GEO_FULL);
}

RenderSubPlanet::~RenderSubPlanet()
{
    //DR_SAVE_DELETE(mGeometrieGrid);
}

DRReturn RenderSubPlanet::render(float fTime, Camera* cam)
{   
    generateAndBindTexture();

    if(mGeometrieGrid->render()) 
        LOG_ERROR("Fehler bei Geometrie Grid", DR_ERROR);
    
    if(DRGrafikError("[RenderSubPlanet::render]")) return DR_ERROR;
    return DR_OK;
}


