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
    getBoxSideEdges(boxSide, mSides);
    mGeometrieGrid->initHeightfield(mSides, 33, mHeights);
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
    int index = 0;
    switch(boxSide)
    {
        case BOX_FRONT: index = 0; break;
        case BOX_RIGHT: index = 1; break;
        case BOX_BACK:  index = 2; break;
        case BOX_LEFT:  index = 3; break;
        case BOX_TOP:   index = 4; break;
        case BOX_BOTTOM:index = 5; break;
        default: index = -1;
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