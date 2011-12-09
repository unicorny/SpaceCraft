#include "main.h"

SubPlanetSektor::SubPlanetSektor(Vector3Unit position, Unit radius, SektorID id, Sektor* parent, int subLevel)
: Sektor(position, radius, id, parent), mSubLevel(subLevel), mHorizontCulling(0.0)
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

SubPlanetSektor::~SubPlanetSektor()
{
    for(int i = 0; i < 4; i++)
    {
        if(mNeighbors[i])
        {
            if(mNeighbors[i] == this) LOG_ERROR_VOID("nachbarpointer zeigt auf sich selbst!");
            for(int j = 0; j < 4; j++)
            {
                if(mNeighbors[i]->mNeighbors[j] == this)
                    mNeighbors[i]->mNeighbors[j] = NULL;
            }
        }
    }
    memset(mNeighbors, 0, sizeof(SubPlanetSektor*)*4);
}


DRReturn SubPlanetSektor::move(float fTime, Camera* cam)
{
    //teilen bei Camera Distance von 1.5 radius
    mLastRelativeCameraPosition = cam->getSektorPositionAtSektor(this);
    mHorizontCulling = acos(mRadius/mLastRelativeCameraPosition.length())*RADTOGRAD;
    if(mParent)
    {
        if(!isObjectInSektor(mLastRelativeCameraPosition))    
            mIdleSeconds += fTime;
        else
            mIdleSeconds = 0.0f;
    }
    
    if(mHorizontCulling <= 70.0)
    {
        //sub sektoren erstellen
        getChild(SektorID(-1, 1, 0));
        getChild(SektorID( 1, 1, 0));
        getChild(SektorID( 1,-1, 0));
        getChild(SektorID(-1,-1, 0));
    }
    else
    {
        removeInactiveChilds(1.0f);
    }

    return DR_OK;
}

DRReturn SubPlanetSektor::render(float fTime, Camera* cam)
{
    if(mIdleSeconds > 0.0f) return DR_OK;
    DRVector3 pos = mSektorPosition.getVector3().normalize();
    glTranslated(pos.x, pos.y, pos.z);

    //GlobalRenderer::getSingleton().getPlanetShaderPtr()->bind();
    if(mHorizontCulling > 70.0)
    {
        mRenderer->render(fTime, cam);
        GlobalRenderer::getSingleton().getPlanetShaderPtr()->unbind();
        // childs didn't need to render
        return DR_NOT_ERROR;
    }
    return DR_OK;
}

bool SubPlanetSektor::isObjectInSektor(Vector3Unit positionInSektor)
{    
    double angle = acos(positionInSektor.normalize().dot(mSektorPosition.getVector3().normalize()));   
    if(angle*RADTOGRAD > 130)
        return false;
    return true;   
}

Sektor* SubPlanetSektor::getChild(SektorID childID)
{
    return NULL;
}