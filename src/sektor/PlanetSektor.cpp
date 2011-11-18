#include "main.h"

PlanetSektor::PlanetSektor(Vector3Unit position, Unit radius, SektorID id, Sektor* parent)
: Sektor(position, radius, id, parent)
{
    mRenderer = new RenderPlanet();
}

PlanetSektor::~PlanetSektor()
{
    DR_SAVE_DELETE(mParent);
    DR_SAVE_DELETE(mRenderer);
}

DRReturn PlanetSektor::move(float fTime, Camera* cam)
{
     
    return DR_OK;
}

DRReturn PlanetSektor::render(float fTime, Camera* cam)
{
    if(mRenderer)
    {
        glPushMatrix();

        Unit distance1 = Vector3Unit(mSektorPosition - cam->getAbsPosition()).length();
        DRVector3 diff = Vector3Unit(mSektorPosition - cam->getAbsPosition()).convertTo(KM).getVector3();

        distance1 = distance1.convertTo(mRadius.getType());
        float distance2 = 200.0f;
        Unit radius1 = mRadius;
        float radius2 = ((radius1 * distance2) / distance1);

        DRVector3 pos = (mSektorPosition - cam->getAbsPosition()).getVector3().normalize();
        DRVector3 relCameraPos = -pos*distance1/mRadius;
        pos *= distance2;
/*   printf("\r %f %f %f, %.8f, %s  x:%s y:%s z:%s (%f %f %f)", pos.x, pos.y, pos.z, radius2, distance1.print().data(),
                                           absCameraPosition.x.print().data(), absCameraPosition.y.print().data(),
                                           absCameraPosition.z.print().data(), diff.x, diff.y, diff.z);
                                               //*/


        glTranslatef(pos.x, pos.y, pos.z);
        glScalef(radius2, radius2, radius2);
        GlobalRenderer::getSingleton().getPlanetShaderPtr()->bind();

        DRReturn ret = mRenderer->render(fTime, cam);
        GlobalRenderer::getSingleton().getPlanetShaderPtr()->unbind();
        glPopMatrix();
        if(ret) LOG_ERROR("Fehler bei render sub-sekotr renderer", DR_ERROR);
    }
    
    
    return DR_OK;
}