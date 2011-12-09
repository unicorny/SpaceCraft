#include "main.h"

s8 RenderSubPlanet::BoxValues[][3] = {{-1, 1, 0}, { 1, 1, 0}, {-1,-1, 0}, { 1,-1, 0}, // front 
                                      { 0, 1,-1}, { 0, 1, 1}, { 0,-1,-1}, { 0,-1, 1}, // right 
                                      { 1, 1, 0}, {-1, 1, 0}, { 1,-1, 0}, {-1,-1, 0}, // back
                                      { 0, 1, 1}, { 0, 1,-1}, { 0,-1, 1}, { 0,-1,-1}, // left
                                      {-1, 0, 1}, { 1, 0, 1}, {-1, 0,-1}, { 1, 0,-1}, // top
                                      {-1, 0,-1}, { 1, 0,-1}, {-1, 0, 1}, { 1, 0, 1}, // bottom
                                      {-1, 1, 0}, { 1, 1, 0}, {-1,-1, 0}, { 1,-1, 0}}; // center

RenderSubPlanet::RenderSubPlanet(GenerateNoisePlanet* noiseGenerator,RenderGridBoxSide boxSide, DRVector3 sphericalCenter)
: RenderSektor(), mHeights(NULL), mNoiseGenerator(noiseGenerator), mGeometrieGrid(NULL),
  mUpdateThread(NULL), mUpdateThreadSemaphore(NULL), mUpdateMutex(NULL)
{
    if(!mNoiseGenerator) return;
    mHeights = new PlanetHeightValues(mNoiseGenerator);
    
    double seaLevelInMeters = mNoiseGenerator->getSeaLevelInMetres();
    
    mHeights->ClearGradientPoints ();
    mHeights->AddGradientPoint (-2.0 + seaLevelInMeters, noise::utils::Color (  0,   0,   0, 255));
    mHeights->AddGradientPoint (    -0.03125 + seaLevelInMeters, noise::utils::Color (  6,  58, 127, 255));
    mHeights->AddGradientPoint (    -0.0001220703 + seaLevelInMeters, noise::utils::Color ( 14, 112, 192, 255));
    mHeights->AddGradientPoint (     0.0 + seaLevelInMeters, noise::utils::Color ( 70, 120,  60, 255));
    mHeights->AddGradientPoint (  0.125 + seaLevelInMeters, noise::utils::Color (110, 140,  75, 255));
    mHeights->AddGradientPoint (  0.25 + seaLevelInMeters, noise::utils::Color (160, 140, 111, 255));
    mHeights->AddGradientPoint (  0.375 + seaLevelInMeters, noise::utils::Color (184, 163, 141, 255));
    mHeights->AddGradientPoint (  0.5 + seaLevelInMeters, noise::utils::Color (255, 255, 255, 255));
    mHeights->AddGradientPoint (  0.75 + seaLevelInMeters, noise::utils::Color (128, 255, 255, 255));
    mHeights->AddGradientPoint ( 2.0 + seaLevelInMeters, noise::utils::Color (  0,   0, 255, 255));
    
    mGeometrieGrid = new DRGeometrieHeightfield(sphericalCenter);
    getBoxSideEdges(boxSide, mSides);
    //getBoxSideEdges(BOX_CENTER, mSides);
 /*   float f = 1.1f;
    mSides[0] += (mSides[1] - mSides[0])*f;
    mSides[2] += (mSides[3] - mSides[2])*f;
    
    mSides[1] += (mSides[3] - mSides[1])*f;
    mSides[0] += (mSides[2] - mSides[0])*f;
   //*/ 
    //if(boxSide == BOX_FRONT)
        //mGeometrieGrid->initHeightfield(mSides, 600, mHeights);
    //else
    mGeometrieGrid->initHeightfield(mSides, 33, mHeights);
        
}

RenderSubPlanet::~RenderSubPlanet()
{
    DR_SAVE_DELETE(mHeights);
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

DRReturn RenderSubPlanet::getBoxSideEdges(RenderGridBoxSide boxSide, DRVector3 edges[4])
{
    int index = 0;
    switch(boxSide)
    {
        case BOX_FRONT: index = 0; break;
        case BOX_RIGHT: index = 1; break;
        case BOX_BACK:  index = 2; break;
        case BOX_LEFT:  index = 3; break;
        case BOX_TOP:   index = 4; break;
        case BOX_BOTTOM:index = 5; break;
        case BOX_CENTER:index = 6; break;
        default: index = 0;
    }
    for(int i = 0; i < 3; i++)
    {
        for(int edge = 0; edge < 4; edge++)
        {
            edges[edge].c[i] = BoxValues[index*4+edge][i];
        }
/*		edges[0].c[i] = BoxValues[index*4][i];
		edges[1].c[i] = BoxValues[index*4+1][i];
		edges[2].c[i] = BoxValues[index*4+2][i];
		edges[3].c[i] = BoxValues[index*4+3][i];
		*/
    }
    return DR_OK;
}

DRReturn RenderSubPlanet::getCurrentSideEdges(DRVector3* out)
{
    if(!out) return DR_ZERO_POINTER;
    memcpy(out, mSides, sizeof(DRVector3)*4);
}