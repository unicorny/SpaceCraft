#include "main.h"

SubPlanetSektor::SubPlanetSektor(Vector3Unit position, Unit radius, SektorID id, Sektor* parent, int subLevel)
: Sektor(position, radius, id, parent), mSubLevel(subLevel)
{
    mType = SUB_PLANET;  
    memset(mNeighbors, 0, sizeof(SubPlanetSektor*)*4);
    PlanetSektor* parentPlanet = (PlanetSektor*)parent;
    if(parentPlanet->getType() == PLANET)
    {
        RenderGridBoxSide boxSide;    
        if(id.z == -1) boxSide = BOX_FRONT;
        else if(id.x == 1) boxSide = BOX_RIGHT;
        else if(id.z == 1) boxSide = BOX_BACK;
        else if(id.x == -1) boxSide = BOX_LEFT;
        else if(id.y == 1) boxSide = BOX_TOP;
        else boxSide = BOX_BOTTOM;
        mRenderer = new RenderSubPlanet(parentPlanet->getNoiseGenerator(), boxSide, -position.getVector3().normalize());
        
        mSubLevel = 6;
    }
}

DRReturn SubPlanetSektor::move(float fTime, Camera* cam)
{
    mLastRelativeCameraPosition = cam->getSektorPositionAtSektor(this);
    if(mParent)
    {
        if(!mParent->isObjectInSektor(mLastRelativeCameraPosition))
            mIdleSeconds += fTime;
        else
            mIdleSeconds = 0.0f;
    }

    return DR_OK;
}

DRReturn SubPlanetSektor::render(float fTime, Camera* cam)
{
    DRVector3 pos = mSektorPosition.getVector3().normalize();
    glTranslated(pos.x, pos.y, pos.z);

    //GlobalRenderer::getSingleton().getPlanetShaderPtr()->bind();
    
    mRenderer->render(fTime, cam);
    GlobalRenderer::getSingleton().getPlanetShaderPtr()->unbind();
    return DR_OK;
}

bool SubPlanetSektor::isObjectInSektor(Vector3Unit positionInSektor)
{    
    Unit radiusSquare = mRadius.convertTo(KM)*1.5;
    radiusSquare *= radiusSquare;
    return positionInSektor.convertTo(KM).lengthSq() <= radiusSquare;    
}