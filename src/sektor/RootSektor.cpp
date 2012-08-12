#include "RootSektor.h"
#include "GlobalRenderer.h"

RootSektor::RootSektor(SektorID id)
: Sektor(Vector3Unit(0.0), Unit(0.0, M), id, NULL)
{
    mType = SEKTOR_ROOT;
}

RootSektor::~RootSektor()
{
    
}

void RootSektor::addSektor(Sektor* sektor, SektorID id)
{
    mChilds.insert(SEKTOR_ENTRY(id, sektor));
}

DRReturn RootSektor::move(float fTime, Camera* cam)
{
    mLastRelativeCameraPosition = cam->getSektorPositionAtSektor(this);
    return DR_OK;
}

DRReturn RootSektor::render(float fTime, Camera* cam)
{
	GlobalRenderer& gr = GlobalRenderer::Instance();
	gr.setProjectionMatrix(DRMatrix::perspective_projection(45.0f, static_cast<GLfloat>(XWIDTH/YHEIGHT), 0.01f, 1000.0f));
	
    return DR_OK;
}