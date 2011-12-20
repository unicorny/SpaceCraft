#include "main.h"

SubPlanetSektor::SubPlanetSektor(Vector3Unit position, Unit radius, SektorID id, Sektor* parent, PlanetSektor* planet, DRVector3 edgePoints[4], int subLevel)
: Sektor(position, radius, id, parent), mSubLevel(subLevel), mHorizontCulling(0.0), mPlanet(planet)
{
    mType = SUB_PLANET;  
    memset(mNeighbors, 0, sizeof(SubPlanetSektor*)*4);
    memset(mEdgePoints, 0, sizeof(DRVector3)*4);
    if(!edgePoints)
        fillEdgePoints();
    else 
        memcpy(mEdgePoints, edgePoints, sizeof(DRVector3)*4);
        
    mRenderer = new RenderSubPlanet(planet->getNoiseGenerator(), planet->getHeightValues(), mEdgePoints, -position.getVector3().normalize());
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
	mHorizontCulling = 75.0;
    if(mSubLevel != 6) return DR_OK;
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
        getChild(SektorID(-1, 1, 0));  // rechts unten
        //getChild(SektorID( 1, 1, 0));  // links unten
      //  getChild(SektorID( 1,-1, 0));
      //  getChild(SektorID(-1,-1, 0));
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
    if(mSubLevel == 6)
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
    if(mChilds.find(childID) == mChilds.end())
    {
        // subPlanet seiten eines Würfels mit Kantenlänge 2 (intern)
        
        DRVector3 edgePoints[4];
        calculateNewEdgePoints(childID, edgePoints);
        DRVector3 center = edgePoints[0] + (edgePoints[1] - edgePoints[0])/2.0f
                                         + (edgePoints[2] - edgePoints[0])/2.0f;
        
        DRVector3 dir = DRVector3(center+mSektorPosition.getVector3().normalize());
        float length = dir.length();
        DRVector3 centerNormalized = center +  (dir / length) * (1.0f-length);
        
        // Position des Quadratmittelpunktes
        Vector3Unit position = Vector3Unit(centerNormalized*mRadius)+mSektorPosition;
        position = position.convertTo(KM);
        position.print("planet pos"); printf("subLevel: %d, %s\n", mSubLevel, position.length().print().data());

        Unit radius = mRadius;// * faktorH;
        //printf("radius: %s\n", radius.print().data());
        SubPlanetSektor* temp = new SubPlanetSektor(position, radius, childID, this, mPlanet, edgePoints, mSubLevel + 1);
        mChilds.insert(SEKTOR_ENTRY(childID, temp));

        //Set neighbor pointer
        for(int i = 0; i < 4; i++)
        {
            
        }
    }
    //*/
    return mChilds[childID];
}

void SubPlanetSektor::fillEdgePoints()
{
    s8 boxValues[][3] = {{-1, 1, 0}, { 1, 1, 0}, {-1,-1, 0}, { 1,-1, 0}, // front 
                         { 0, 1,-1}, { 0, 1, 1}, { 0,-1,-1}, { 0,-1, 1}, // right 
                         { 1, 1, 0}, {-1, 1, 0}, { 1,-1, 0}, {-1,-1, 0}, // back
                         { 0, 1, 1}, { 0, 1,-1}, { 0,-1, 1}, { 0,-1,-1}, // left
                         {-1, 0, 1}, { 1, 0, 1}, {-1, 0,-1}, { 1, 0,-1}, // top
                         {-1, 0,-1}, { 1, 0,-1}, {-1, 0, 1}, { 1, 0, 1}, // bottom
                         {-1, 1, 0}, { 1, 1, 0}, {-1,-1, 0}, { 1,-1, 0}}; // center
    
    RenderGridBoxSide boxSide;    
    if(mID.z == -1) boxSide = BOX_FRONT;
    else if(mID.x == 1) boxSide = BOX_RIGHT;
    else if(mID.z == 1) boxSide = BOX_BACK;
    else if(mID.x == -1) boxSide = BOX_LEFT;
    else if(mID.y == 1) boxSide = BOX_TOP;
    else boxSide = BOX_BOTTOM;
        
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
            mEdgePoints[edge].c[i] = boxValues[index*4+edge][i];
        }
    }
}

DRReturn SubPlanetSektor::getCurrentSideEdges(DRVector3 out[4])
{
    if(!out) return DR_ZERO_POINTER;
    memcpy(out, mEdgePoints, sizeof(DRVector3)*4);
    return DR_OK;
}

void SubPlanetSektor::calculateNewEdgePoints(SektorID index, DRVector3 edgePoints[4])
{
    for(int i = 0; i < 4; i++)
        edgePoints[i] = mEdgePoints[i]/2.0f;
  //  return;
    DRVector3 xVectorPart = (mEdgePoints[1]-mEdgePoints[0])/2;
    DRVector3 yVectorPart = (mEdgePoints[2]-mEdgePoints[0])/2;
    DRVector3 center = mEdgePoints[0] + xVectorPart + yVectorPart;
    
    //DRVector3 dir = DRVector3(center+mSektorPosition.getVector3().normalize());
    //float length = dir.length();
  //  center += (dir / length) * (1.0f-length);
       
    if(index == SektorID(-1, 1, 0)) // oben links // rechts unten
    {
        edgePoints[0] = center;
        edgePoints[1] = center + xVectorPart;
        edgePoints[2] = center               + yVectorPart;
        edgePoints[3] = center + xVectorPart + yVectorPart;
        //edgePoints[0] = center - xVectorPart + yVectorPart;
        //edgePoints[1] = center               + yVectorPart;
        //edgePoints[2] = center - xVectorPart;
        //edgePoints[3] = center;
    }
    else if(index == SektorID(1, 1, 0)) // rechts oben
    {
        edgePoints[0] = center               + yVectorPart;
        edgePoints[1] = center + xVectorPart + yVectorPart;
        edgePoints[2] = center;
        edgePoints[3] = center + xVectorPart;
    }
    else if(index == SektorID(1, -1, 0)) // rechts unten
    {
        edgePoints[0] = center;
        edgePoints[1] = center + xVectorPart;
        edgePoints[2] = center               - yVectorPart;
        edgePoints[3] = center + xVectorPart - yVectorPart;
    }
    else if(index == SektorID(-1, -1, 0)) // links unten
    {
        edgePoints[0] = center - xVectorPart;
        edgePoints[1] = center;
        edgePoints[2] = center - xVectorPart - yVectorPart;
        edgePoints[3] = center               - yVectorPart;
    }
    
    for(int i = 0; i < 4; i++)
    {
        DRVector3 dir = DRVector3(edgePoints[i]-mSektorPosition.getVector3().normalize());
        float length = dir.length();
        //edgePoints[i] += (dir / length) * (1.0f-length);
    }
}