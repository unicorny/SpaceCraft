#include "SubPlanetSektor.h"
#include "GlobalRenderer.h"
#include "SubPatchPlanetSektor.h"

SubPlanetSektor::SubPlanetSektor(Vector3Unit position, Unit radius, SektorID id, Sektor* parent, SektorPtr planet,
                    float patchScaling/* = 0.0f*/, int subLevel/* = 6*/)
: Sektor(position, radius, id, parent), mSubLevel(subLevel), mPlanet(planet),
  mPatchScaling(patchScaling), mVectorToPlanetCenter(position.getVector3().normalize()), mHorizontCulling(0.0)
{
    mType = SUB_PLANET;  
    DRVector3 childPos(mID.x, mID.y, mID.z);
    DRMatrix rotation;
    childPos /= 1000.0f;
    memset(mNeighbors, 0, sizeof(SubPlanetSektor*)*4);
    if(!mParent || !mPlanet.getResourcePtrHolder())
        LOG_ERROR_VOID("no parent, no planet, nothing");
    
    //berechnen der Rotationsmatrix für die Texturgenerierung
    DRVector3 centerPosition = position.getVector3().normalize(); 
    DRVector3 startAxis(0.00001f, 0.00001f, 1.0f);
   
    DRVector3 rotationAxis = startAxis.cross(centerPosition).normalize();//startAxis.cross(centerPosition).normalize();
    float rotationAngle = startAxis.dot(centerPosition);//startAxis.dot(centerPosition);

    Eigen::Affine3f affine;
    affine = Eigen::AngleAxisf(acosf(rotationAngle), Eigen::Vector3f(rotationAxis));
            //*Eigen::AngleAxisf(acosf(5.0f*GRADTORAD*faktor), Eigen::Vector3f(0.0f, 0.0f, 1.0f));

    mRotation = DRMatrix(affine.data());
    printf("[SubPlanetSektor::SubPlanetSektor] patchScaling: %f\n", mPatchScaling);
    //mRotation = DRMatrix::identity();
    //mSphericalCenter = mSphericalCenter.transformCoords(mRotation.invert() );
    printf("[SubPlanetSektor::SubPlanetSektor] center: %f %f %f\n", centerPosition.x, centerPosition.y, centerPosition.z);
    position.print("[SubPlanetSektor::SubPlanetSektor] position");
    
    if(subLevel > 1)
    {
        rotation = dynamic_cast<SubPlanetSektor*>(mParent)->getRotation();
        //mRotation = mRotation * rotation;
        mVectorToPlanetCenter = (mParent->getPosition()+mSektorPosition).getVector3().normalize();
    }
    if(subLevel == 1)
        mRenderer = new RenderSubPlanet(id, DRVector3(0.0f), patchScaling, mRotation, getSektorPathName(), getPlanet()->getPlanetNoiseParameters());
    else
        mRenderer = new RenderSubPlanet(id, childPos, patchScaling, rotation, getSektorPathName(), getPlanet()->getPlanetNoiseParameters());
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
    //DRLog.writeToLog("deconstruct SubPlanetSektor");
}

void SubPlanetSektor::release()
{
    mPlanet.release();
    Sektor::release();
}


DRReturn SubPlanetSektor::move(float fTime, Camera* cam)
{
    mHorizontCulling = 75.0;
    float inactiveTime = GlobalRenderer::Instance().getTimeForInactiveChilds();
    RenderSubPlanet* render = dynamic_cast<RenderSubPlanet*>(mRenderer);
    //if(mSubLevel != 1) return DR_OK;
    //teilen bei Camera Distance von 1.5 radius
    mLastRelativeCameraPosition = cam->getSektorPositionAtSektor(mThis).convertTo(KM);
    DRVector3 patchPosition = mSektorPosition.getVector3().normalize();
    DRVector3 cameraPosition = mLastRelativeCameraPosition.getVector3().normalize();
    mHorizontCulling = acos(cameraPosition.dot(patchPosition))*RADTOGRAD;    
    //mLastRelativeCameraPosition.print("[SubPlanetSektor::move] camPos");
   // printf(" radius: %s\n", mRadius.print().data());
    //printf("\rhorizont: %f, theta: %f", mHorizontCulling, mPlanet->getTheta());
    if(mParent)
    {
        if(!isObjectInSektor(mLastRelativeCameraPosition))    
            mIdleSeconds += fTime;
        else
            mIdleSeconds = 0.0f;
    }
    if(mSubLevel == 1 && getPlanet()->getTheta() <= 50.0*GRADTORAD && mHorizontCulling < 120.0f)// ||
       //mSubLevel == 2 && getPlanet()->getTheta() <= 0.523 && mHorizontCulling < 100.0f)
    {
        short value = 500;
        if(mSubLevel == 2) value = 250;
        //sub sektoren erstellen
        getChild(SektorID( value,  value, value*2)); // rechts unten
        getChild(SektorID( value, -value, value*2)); // links unten
        getChild(SektorID(-value,  value, value*2)); //rechts oben
        getChild(SektorID(-value, -value, value*2)); //rechts unten
    //* */
        
        //0,0,-1
/*        getChild(SektorID(250, 250, -750));
        getChild(SektorID(250, -250, -750));
        getChild(SektorID(-250, 250, -750));
        getChild(SektorID(-250, -250, -750));
        //*/
        

    }
    else
    {
        removeInactiveChilds(inactiveTime);
    }
    if(render && render->getRenderNoisePlanetToTexture())
    {
		float distance = static_cast<float>(mLastRelativeCameraPosition.length().convertTo(M));
        if(mIdleSeconds > 0.0) distance *= 1000.0;
        dynamic_cast<RenderSubPlanet*>(mRenderer)->getRenderNoisePlanetToTexture()->setCurrentDistance(distance);
        if(mNotRenderSeconds > inactiveTime)
            DR_SAVE_DELETE(mRenderer);
    }

    return DR_OK;
}

DRReturn SubPlanetSektor::render(float fTime, Camera* cam)
{
    if(mIdleSeconds > 0.0f) return DR_NOT_ERROR;
    DRVector3 childPos(mID.x, mID.y, mID.z);
    childPos /= 1000.0f;
    if(mSubLevel == 1)
    {
        mMatrix = mRotation * mParent->getMatrix();
    }
    else
    {
        mMatrix = mParent->getMatrix();
    }
       
    
    if(mIdleSeconds <= 0.0f &&
      (mSubLevel == 1 && (mHorizontCulling > 120.0f || getPlanet()->getTheta() > 50.0*GRADTORAD)) ||
      (mSubLevel == 2 && (mHorizontCulling > 100.0f || getPlanet()->getTheta() > 0.523 )) || //ca. 30 Grad
       mSubLevel == 3 )//&& (mHorizontCulling > 100.0f || getPlanet()->getTheta() > 0.523)) 
    {
        mNotRenderSeconds = 0.0f;
        if(!mRenderer)
            mRenderer = new RenderSubPlanet(mID, DRVector3(0.0f), mPatchScaling, mRotation, getSektorPathName(), getPlanet()->getPlanetNoiseParameters());
        if(!mRenderer) LOG_ERROR("no renderer", DR_ERROR);
        
        ShaderProgram* shader = mRenderer->getShaderProgram();
        const PlanetNoiseParameter* p = getPlanet()->getPlanetNoiseParameters();
        if(!shader) LOG_ERROR("renderer shader isn't valid", DR_ERROR);
        shader->bind();
        shader->setUniformMatrix("projection", GlobalRenderer::Instance().getProjectionMatrix());
        shader->setUniformMatrix("modelview", mMatrix);
        if(mSubLevel == 1)
            shader->setUniform3fv("translate", DRVector3(0.0f));
        else
            shader->setUniform3fv("translate", childPos);
            
        shader->setUniform1f("patchScaling", mPatchScaling);
        shader->setUniform1f("MAX_HEIGHT_IN_PERCENT", p->maxHeightInPercent);
        shader->setUniform1f("MIN_HEIGHT_IN_PERCENT", p->minHeightInPercent);
        mRenderer->render(fTime, cam);
        shader->unbind();
        //GlobalRenderer::getSingleton().getPlanetShaderPtr()->unbind();
        // childs didn't need to render
        return DR_NOT_ERROR;
      
    }
    else
    {
        mNotRenderSeconds += fTime;
    }
    return DR_OK;
}

bool SubPlanetSektor::isObjectInSektor(Vector3Unit positionInSektor)
{    
    //if(mPlanet->getTheta() >= 1.2218) return false;
    double theta = (mRadius/positionInSektor.length());
	if(mSubLevel == 1)
        if(theta <= 0.3060) return false;
    //if(mSubLevel == 2)
        //if(theta <= 1.54) return false;
        //printf("\rtheta: %.5f", theta);
    
    DRVector3 posInSektorNorm = positionInSektor.getVector3().normalize();
    DRVector3 sektorPosNorm = mSektorPosition.getVector3().normalize();
    float d = posInSektorNorm.dot(mVectorToPlanetCenter);
    double angle = acos(posInSektorNorm.dot(mVectorToPlanetCenter));   
  /*  printf("\r angle: %f (dot: %f), idle: %f, sektorPos: %f, %f, %f, posInSektor: %f, %f, %f", angle*RADTOGRAD, d, mIdleSeconds, 
                                                                                         sektorPosNorm.x, sektorPosNorm.y, sektorPosNorm.z,
                                                                                         posInSektorNorm.x, posInSektorNorm.y, posInSektorNorm.z);
    //*/
    if(mSubLevel == 1)
    {
        if(angle*RADTOGRAD > 125)
            return false;
    }
    else if(mSubLevel == 2)
    {
        if(angle*RADTOGRAD > 110)
            return false;
    }
    else if(mSubLevel == 3)
    {
        if(angle*RADTOGRAD > 100)
            return false;
    }
    return true;   
}

SektorPtr SubPlanetSektor::getChild(SektorID childID)
{
    if(mChilds.find(childID) == mChilds.end())
    {
        // subPlanet seiten eines Würfels mit Kantenlänge 2 (intern)
        
        // Position des Quadratmittelpunktes
        DRVector3 childPos(childID.x, childID.y, childID.z);
        DRVector3 myPosition = mSektorPosition.getVector3().normalize();
        float newPatchScale = mPatchScaling/2.0f;
        childPos /= 1000.0f;
        childPos = childPos.transformCoords(mRotation).normalize()/(newPatchScale*0.5f);
        Vector3Unit position = Vector3Unit(DRVector3(myPosition + childPos).normalize()*static_cast<float>(mRadius), KM);
        position -= mSektorPosition;
        //position = Vector3Unit(DRVector3(0.0f, 0.0f, -1.0f).transformNormal(mRotation)*static_cast<double>(mRadius), KM);
        //position = position.convertTo(KM);
        position.print("[SubPlanetSektor::getChild] planet pos"); printf("[SubPlanetSektor::getChild] subLevel: %d, %s\n", mSubLevel+1, position.length().print().data());

        //Unit radius = mRadius;// * faktorH;
        //printf("radius: %s\n", radius.print().data());
        //906 ist zu groß (lücken links und rechts)
        SubPlanetSektor* temp = new SubPlanetSektor(position, mRadius, childID, this, mPlanet, newPatchScale, mSubLevel + 1);
        //SubPlanetSektor* temp = dynamic_cast<SubPlanetSektor*>(&(*sektor));
        mChilds.insert(SEKTOR_ENTRY(childID, temp->getThis()));

        //Set neighbor pointer
        for(int i = 0; i < 4; i++)
        {
            
        }
    }
    //*/
    return mChilds[childID];
}

