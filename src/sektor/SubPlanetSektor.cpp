#include "SubPlanetSektor.h"
#include "GlobalRenderer.h"
#include "SubPatchPlanetSektor.h"

DRVector2 SubPlanetSektor::mSubLevelBorder[] = 
{DRVector2(70.0f, 125.0f), // 0
 DRVector2(50.0f, 110.0f), // 1
 DRVector2(30.0f, 100.0f), // 2
 DRVector2(20.0f, 100.0f), // 3
 DRVector2(15.0f,  95.0f), // 4
 DRVector2(10.0f,  95.0f), // 5
 DRVector2(8.0f,  89.0f), // 6
 DRVector2(6.0f,   85.0f)}; // 7
//*/

#define MAX_SUB_LEVEL 8

SubPlanetSektor::SubPlanetSektor(Unit radius, SektorID id, Sektor* parent, PlanetSektor* planet,
                    float patchScaling/* = 0.0f*/, int subLevel/* = 6*/)
: Sektor(Vector3Unit(0.0), radius, id, parent), mSubLevel(subLevel), mPlanet(planet),
  mPatchScaling(patchScaling), mVectorToPlanetCenter(DRVector3(0.0f)),
  mTextureTranslate(0.0f, 0.0f, 1.0f)
{
    mType = SUB_PLANET;  
    DRVector3 childID = mID;
    memset(mNeighbors, 0, sizeof(SubPlanetSektor*)*4);
    if(!mParent || !mPlanet)
        LOG_ERROR_VOID("no parent, no planet, nothing");
    
    //printf("[SubPlanetSektor::SubPlanetSektor] patchScaling: %f\n", mPatchScaling);
    if(mSubLevel == 1)
    {
        mSektorPosition = Vector3Unit(childID.normalize(), KM)*mRadius;
        mVectorToPlanetCenter = childID.normalize();
        //berechnen der Rotationsmatrix für die Texturgenerierung
        DRVector3 centerPosition = mSektorPosition.getVector3().normalize(); 
        DRVector3 startAxis(0.00001f, 0.00001f, 1.0f);

        DRVector3 rotationAxis = startAxis.cross(centerPosition).normalize();//startAxis.cross(centerPosition).normalize();
        float rotationAngle = startAxis.dot(centerPosition);//startAxis.dot(centerPosition);

        Eigen::Affine3f affine;
        affine = Eigen::AngleAxisf(acosf(rotationAngle), Eigen::Vector3f(rotationAxis));
                //*Eigen::AngleAxisf(acosf(5.0f*GRADTORAD*faktor), Eigen::Vector3f(0.0f, 0.0f, 1.0f));

        mRotation = DRMatrix(affine.data());
        //printf("[SubPlanetSektor::SubPlanetSektor] center: %f %f %f\n", centerPosition.x, centerPosition.y, centerPosition.z);
    }
    else
    {
        SubPlanetSektor* parentSektor = dynamic_cast<SubPlanetSektor*>(mParent);
        mRotation = parentSektor->getRotation();
        //Vector3Unit parentVector = mParent->getRadius() * parentSektor->getVectorToPlanet();
        //mVectorToPlanetCenter = (parentVector+mSektorPosition).getVector3().normalize();
        //mVectorToPlanetCenter
        if(mSubLevel > 2)
            childID = childID / powf(2.0f, static_cast<float>(mSubLevel-2));
		printf("[SubPlanetSektor::SubPlanetSektor] id: %f %f %f\n", childID.x, childID.y, childID.z);
        //printf("[SubPlanetSektor::SubPlanetSektor] )
        mTextureTranslate = parentSektor->getTextureTranslate() + childID;
        mVectorToPlanetCenter = mTextureTranslate.transformCoords(mRotation).normalize();
        
        //mTextureTranslate = parentSektor->getTextureTranslate() + childPos;
        
        mSektorPosition = (mRadius * mVectorToPlanetCenter) - (mRadius * parentSektor->getVectorToPlanet());
    }
    printf("[SubPlanetSektor::SubPlanetSektor] mVectorToPlanetCenter: %f %f %f\n", mVectorToPlanetCenter.x, mVectorToPlanetCenter.y, mVectorToPlanetCenter.z);
    printf("[SubPlanetSektor::SubPlanetSektor] mTextureTranslate: %f %f %f\n", mTextureTranslate.x, mTextureTranslate.y, mTextureTranslate.z);
    mSektorPosition.print("[SubPlanetSektor::SubPlanetSektor] position");
    printf("[SubPlanetSektor::SubPlanetSektor] subLevel: %d, position.length: %s\n", mSubLevel, mSektorPosition.length().print().data());
    mParent->printTypeInfos("[SubPlanetSektor::SubPlanetSektor] parent ");
 
    mRenderer = new RenderSubPlanet(id, mTextureTranslate, patchScaling, mRotation, getSektorPathName(), mPlanet->getPlanetNoiseParameters());
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
   // DRLog.writeToLog("SubPlanetSektor::~SubPlanetSektor");
}


DRReturn SubPlanetSektor::move(float fTime, Camera* cam)
{
    Sektor::move(fTime, cam);
    if(!mPlanet) LOG_ERROR("planet is zero", DR_ZERO_POINTER);
    
    float inactiveTime = GlobalRenderer::Instance().getTimeForInactiveChilds();
    RenderSubPlanet* render = dynamic_cast<RenderSubPlanet*>(mRenderer);    
    double horizontCulling = acos(mLastRelativeCameraPosition.getVector3().normalize().dot(mVectorToPlanetCenter))*RADTOGRAD;   
    
    if(mSubLevel == 3)
    {
        DRVector3 cam = mLastRelativeCameraPosition.convertTo(KM).getVector3();
        //printf("\r cam: %f %f %f", cam.x, cam.y, cam.z);
    }
    
    if(mIdleSeconds <= 0.0f)
    {
        //if child visible                           50 Grad                         120 Grad
        if(mSubLevel > 0 && mSubLevel < MAX_SUB_LEVEL &&
           mPlanet->getTheta() <= mSubLevelBorder[mSubLevel].x && horizontCulling < mSubLevelBorder[mSubLevel-1].y)
     /*   if( (mSubLevel == 1 && mPlanet->getTheta() <= 50.0 && horizontCulling < 125.0) ||
            (mSubLevel == 2 && mPlanet->getTheta() <= 30.0 && horizontCulling < 110.0f) ||
            (mSubLevel == 3 && mPlanet->getTheta() <= 15.0 && horizontCulling < 100.0f))
      * */
        {
            mNotRenderSeconds += fTime;
            short value = 500;
          /*  if(mSubLevel == 2) value = 250;
            else if(mSubLevel == 3) value = 125;
            else if(mSubLevel == 4) value = 62;
             */
            //value = 0;
            //sub sektoren erstellen
            SektorID childId[4] = {SektorID( value,  value, 0),
                                   SektorID( value, -value, 0),
                                   SektorID(-value,  value, 0),
                                   SektorID(-value, -value, 0)};
            for(int i = 0; i < 4; i++)
            {
                DRVector3 planetVector = DRVector3(mTextureTranslate + childId[i]).transformCoords(mRotation).normalize();
                Vector3Unit posVector = (mRadius * planetVector) - (mRadius * mVectorToPlanetCenter);
                DRVector3 camVector = Vector3Unit(mLastRelativeCameraPosition - posVector).getVector3().normalize();
                camVector = mPlanet->getCameraPosition().getVector3().normalize();
                double angle = acos(camVector.dot(planetVector))*RADTOGRAD;
                
                //if(mSubLevel <= 3)
				angle -= 45.0f;
                //if(mSubLevel <= 5) angle -= 34.0f;
                //else angle -= 25.0f;
                
                if(angle < mPlanet->getTheta())
                //if(mSubLevel >= 3 || angle <= mSubLevelBorder[mSubLevel-1].y)
                    getChild(childId[i]);
				//printf("\r angle: %f, theta: %f", angle, mPlanet->getTheta());
            }
        }
        else
        {
            mNotRenderSeconds = 0.0f;        
        }
    }
    else
    {
        mNotRenderSeconds += fTime;
        removeInactiveChilds(inactiveTime);
    }
    
    if(render && render->getRenderNoisePlanetToTexture())
    {
        DRReal distance = static_cast<DRReal>(mLastRelativeCameraPosition.length().convertTo(M));
        if(mIdleSeconds > 0.0f) distance *= 1000.0f;
        dynamic_cast<RenderSubPlanet*>(mRenderer)->getRenderNoisePlanetToTexture()->setCurrentDistance(distance);
        if(mNotRenderSeconds > inactiveTime)
            DR_SAVE_DELETE(mRenderer);
    }

    return DR_OK;
}

DRReturn SubPlanetSektor::render(float fTime, Camera* cam)
{
    if(mIdleSeconds > 0.0f) return DR_NOT_ERROR;
    
    //DRVector3 childPos(mID.x, mID.y, mID.z);
    //childPos /= 1000.0f;
    if(mSubLevel == 1)
        mMatrix = mRotation * mParent->getMatrix();
    else
        mMatrix = mParent->getMatrix();
       
    if(mNotRenderSeconds <= 0.0f)
    {
        if(!mRenderer)
        {
            mRenderer = new RenderSubPlanet(mID, mTextureTranslate, mPatchScaling, mRotation, getSektorPathName(), mPlanet->getPlanetNoiseParameters());
        }
        if(!mRenderer) LOG_ERROR("no renderer", DR_ERROR);
        
        ShaderProgram* shader = mRenderer->getShaderProgram();
        const PlanetNoiseParameter* p = mPlanet->getPlanetNoiseParameters();
        if(!shader) LOG_ERROR("renderer shader isn't valid", DR_ERROR);
        shader->bind();
        shader->setUniformMatrix("projection", GlobalRenderer::Instance().getProjectionMatrix());
        shader->setUniformMatrix("modelview", mMatrix);
        shader->setUniform3fv("translate", mTextureTranslate);
        shader->setUniform1f("patchScaling", mPatchScaling);
        shader->setUniform1f("MAX_HEIGHT_IN_PERCENT", p->maxHeightInPercent);
        shader->setUniform1f("MIN_HEIGHT_IN_PERCENT", p->minHeightInPercent);
        mRenderer->render(fTime, cam);
        shader->unbind();
        //GlobalRenderer::getSingleton().getPlanetShaderPtr()->unbind();
        // childs didn't need to render
        return DR_NOT_ERROR;
    }
    return DR_OK;
}

bool SubPlanetSektor::isObjectInSektor(Vector3Unit positionInSektor)
{    
    if(mSubLevel > 0 &&
       mPlanet->getTheta() >= mSubLevelBorder[mSubLevel-1].x) return false;
    /*if(mSubLevel == 1 && mPlanet->getTheta() >= 70.0f ||
       mSubLevel == 2 && mPlanet->getTheta() >= 50.0f ||
       mSubLevel == 3 && mPlanet->getTheta() >= 30.0f ||
       mSubLevel == 4 && mPlanet->getTheta() >= 15.0f) return false;
     */
    //double theta = (mRadius/positionInSektor.length());
    //if(mSubLevel == 3)
	//	printf("\r theta: %f (%f Grad)", theta, theta*RADTOGRAD);
    //if(theta <= 0.5109/*0.3060*/) return false;
    
    DRVector3 posInSektorNorm = positionInSektor.getVector3().normalize();
    //DRVector3 sektorPosNorm = mSektorPosition.getVector3().normalize();
    float d = posInSektorNorm.dot(mVectorToPlanetCenter);
    double angle = acos(posInSektorNorm.dot(mVectorToPlanetCenter))*RADTOGRAD;   
   /* printf("\r angle: %f (dot: %f), idle: %f, sektorPos: %f, %f, %f, posInSektor: %f, %f, %f", angle*RADTOGRAD, d, mIdleSeconds, 
                                                                                         sektorPosNorm.x, sektorPosNorm.y, sektorPosNorm.z,
                                                                                         posInSektorNorm.x, posInSektorNorm.y, posInSektorNorm.z);
    //*/ 
    //angle = horizontCulling
    if(angle > mSubLevelBorder[mSubLevel-1].y) return false;
    /*if(mSubLevel == 1)
    {
        if(angle > 125.0)
            return false;
    }
    else if(mSubLevel == 2)
    {
        //printf("\r angle: %f Grad", angle);
        if(angle > 110.0)
            return false;        
    }
    else if(mSubLevel == 3)
    {
        if(angle > 100.0)
            return false;
       //printf("\r angle: %f Grad", angle);
    }
    else if(mSubLevel == 4)
    {
        if(angle > 90.0)
            return false;
    }*/
    return true;  
}

Sektor* SubPlanetSektor::getChild(SektorID childID)
{
    if(mChilds.find(childID) == mChilds.end())
    {
        // subPlanet seiten eines Würfels mit Kantenlänge 2 (intern)
        
        // Position des Quadratmittelpunktes
        //if(mSubLevel > 1) myPosition = dynamic_cast<SubPlanetSektor*>(mParent)->getVectorToPlanet();
        DRReal newPatchScale = mPatchScaling/2.0f;
        
        //Unit radius = mRadius;// * faktorH;
        //printf("radius: %s\n", radius.print().data());
        //906 ist zu groß (lücken links und rechts)
        SubPlanetSektor* temp = new SubPlanetSektor(mRadius, childID, this, mPlanet, newPatchScale, mSubLevel + 1);
        mChilds.insert(SEKTOR_ENTRY(childID, temp));

        //Set neighbor pointer
        for(int i = 0; i < 4; i++)
        {
            
        }
    }
    //*/
    return mChilds[childID];
}

void SubPlanetSektor::printTypeInfos(const char* name)
{
    printf("%s SubPlanetSektor, subLevel: %d, id: %d %d %d\n", name, mSubLevel, mID.x, mID.y, mID.z);
}
