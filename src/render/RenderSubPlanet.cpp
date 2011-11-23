#include "main.h"

s8 RenderSubPlanet::BoxValues[][3] = {{-1, 1,-1}, { 1, 1,-1}, {-1,-1,-1}, { 1,-1,-1}, // front 
                                      { 1, 1,-1}, { 1, 1, 1}, { 1,-1,-1}, { 1,-1, 1}, // right 
                                      { 1, 1, 1}, {-1, 1, 1}, { 1,-1, 1}, {-1,-1, 1}, // back
                                      {-1, 1, 1}, {-1, 1,-1}, {-1,-1, 1}, {-1,-1,-1}, // left
                                      {-1, 1, 1}, { 1, 1, 1}, {-1, 1,-1}, { 1, 1,-1}, // top
                                      {-1,-1,-1}, { 1,-1,-1}, {-1,-1, 1}, { 1,-1, 1}};// bottom

RenderSubPlanet::RenderSubPlanet(GenerateNoisePlanet* noiseGenerator, RenderGridBoxSide boxSide)
: RenderPlanet(noiseGenerator), mGeometrieGrid(NULL)
{
    mGeometrieGrid = new DRGeometrieHeightfield(true);
    DRVector3 edges[4];
    getBoxSideEdges(boxSide, edges);
    mGeometrieGrid->initHeightfield(edges, 33, mHeights);
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

DRReturn RenderSubPlanet::getBoxSideEdges(RenderGridBoxSide boxSide, DRVector3 edges[4])
{
    int index = (int)boxSide;
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
