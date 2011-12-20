#include "main.h"

RenderSubPlanet::RenderSubPlanet(GenerateNoisePlanet* noiseGenerator, PlanetHeightValues* heights, DRVector3 boxSideEdges[4], DRVector3 sphericalCenter)
: RenderSektor(), mHeights(heights), mNoiseGenerator(noiseGenerator), mGeometrieGrid(NULL),
  mUpdateThread(NULL), mUpdateThreadSemaphore(NULL), mUpdateMutex(NULL)
{
    if(!mNoiseGenerator || !mHeights) return;
    
    mGeometrieGrid = new DRGeometrieHeightfield(sphericalCenter);
    mGeometrieGrid->initHeightfield(boxSideEdges, 33, mHeights);
}

RenderSubPlanet::~RenderSubPlanet()
{
    DR_SAVE_DELETE(mGeometrieGrid);
}

DRReturn RenderSubPlanet::render(float fTime, Camera* cam)
{   
    //gluSphere(GlobalRenderer::Instance().getQuadric(), 1.0f, 32*2, 32);
    if(mGeometrieGrid->render()) 
        LOG_ERROR("Fehler bei Geometrie Grid", DR_ERROR);
    
    if(DRGrafikError("[RenderSubPlanet::render]")) return DR_ERROR;
    return DR_OK;
}

int RenderSubPlanet::TextureGenerateThread(void* data)
{
    return 0;
}
