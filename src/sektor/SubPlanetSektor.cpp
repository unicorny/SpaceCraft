#include "main.h"

double SubPlanetSektor::Wurzel_3 = sqrtf(3.0f);

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
        mRenderer = new RenderSubPlanet(parentPlanet->getNoiseGenerator(), boxSide, -position.getVector3());
        
        mSubLevel = 6;
    }
}

DRReturn SubPlanetSektor::move(float fTime, Camera* cam)
{
    if(mParent)
    {
        if(!mParent->isObjectInSektor(cam->getSektorPosition()))
            mIdleSeconds += fTime;
        else
            mIdleSeconds = 0.0f;
    }
    return DR_OK;
}

DRReturn SubPlanetSektor::render(float fTime, Camera* cam)
{
     double faktor = 0.5;//mRadius;//-1.0/Wurzel_3;//mRadius;///2.0f;//1.0/Wurzel_3;
     glScaled(faktor, faktor, faktor);
     glTranslated(mSektorPosition.x, mSektorPosition.y, mSektorPosition.z);
 
     DRVector3 pos = mSektorPosition.getVector3().normalize();
     
     //DRLog.writeVector3ToLog(pos, "Position");
     float angle = acos(pos.dot(DRVector3(0.0f, 0.0f, 1.0f)))*RADTOGRAD;
     DRVector3 rotationAxis = pos.cross(DRVector3(0.0f, 0.0f, 1.0f)).normalzeEx();
     if(rotationAxis.lengthSq() < 0.00000001f)
         rotationAxis = DRVector3(0.0f, -1.0f, 0.0f);
     //pos = achse.transformNormal(DRMatrix::rotationAxis(rotationAxis, angle*GRADTORAD));
     
     //glRotatef(angle, rotationAxis.x, rotationAxis.y, rotationAxis.z);

    //GlobalRenderer::getSingleton().getPlanetShaderPtr()->bind();
    
    mRenderer->render(fTime, cam);
    GlobalRenderer::getSingleton().getPlanetShaderPtr()->unbind();
    return DR_OK;
}