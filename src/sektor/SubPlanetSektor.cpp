#include "SubPlanetSektor.h"
#include "GlobalRenderer.h"
#include "SubPatchPlanetSektor.h"
#include "Player.h"
#include "main.h"

DRVector3 SubPlanetSektor::mSubLevelBorder[] = 
{DRVector3(70.0f, 105.0f, 2.2f), // 0 
 DRVector3(50.0f, 102.0f, 1.0f), // 1 
 DRVector3(30.0f, 101.0f, 0.9f), // 2 
 DRVector3(20.0f, 100.0f, 0.45f), // 3
 DRVector3(15.0f, 100.0f, 0.30f), // 4
 DRVector3(10.0f,  99.0f, 0.1f), // 5
 DRVector3(8.0f,   95.0f, 0.04f), // 6
 DRVector3(6.0f,   95.0f, 0.03f),// 7
 DRVector3(4.0f,   94.0f, 0.01f), // 8
 DRVector3(2.0f,   93.0f, 0.009f), // 9
 DRVector3(1.0f,   92.0f, 0.0035f)}; // 10
//*/

#define MAX_SUB_LEVEL 11

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
    
    GlobalRenderer::Instance().addEbenenCount(subLevel);
    
    //printf("[SubPlanetSektor::SubPlanetSektor] patchScaling: %f\n", mPatchScaling);
    if(mSubLevel == 1)
    {
        mVectorToPlanetCenter = childID.normalize();
        mSektorPosition = Vector3Unit(mVectorToPlanetCenter, KM)*mRadius;
        
        //berechnen der Rotationsmatrix für die Texturgenerierung
        DRVector3 centerPosition = mVectorToPlanetCenter; 
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
		//printf("[SubPlanetSektor::SubPlanetSektor] id: %f %f %f\n", childID.x, childID.y, childID.z);
        //printf("[SubPlanetSektor::SubPlanetSektor] )
        mTextureTranslate = parentSektor->getTextureTranslate() + childID;
        mVectorToPlanetCenter = mTextureTranslate.transformCoords(mRotation).normalize();
        
        //mTextureTranslate = parentSektor->getTextureTranslate() + childPos;
        
        mSektorPosition = (mRadius * mVectorToPlanetCenter) - (mRadius * parentSektor->getVectorToPlanet());
    }
    /*printf("[SubPlanetSektor::SubPlanetSektor] mVectorToPlanetCenter: %f %f %f\n", mVectorToPlanetCenter.x, mVectorToPlanetCenter.y, mVectorToPlanetCenter.z);
    printf("[SubPlanetSektor::SubPlanetSektor] mTextureTranslate: %f %f %f\n", mTextureTranslate.x, mTextureTranslate.y, mTextureTranslate.z);
    mSektorPosition.print("[SubPlanetSektor::SubPlanetSektor] position");
    printf("[SubPlanetSektor::SubPlanetSektor] subLevel: %d, position.length: %s\n", mSubLevel, mSektorPosition.length().print().data());
    mParent->printTypeInfos("[SubPlanetSektor::SubPlanetSektor] parent ");
     */
 
    //mRenderer = new RenderSubPlanet(id, mTextureTranslate, patchScaling, mRotation, getSektorPathName(), mPlanet->getPlanetNoiseParameters());
    DRTexturePtr parentTexture;
    if(mSubLevel > 1 && mParent && mParent->getRenderer())
    {
        RenderSubPlanet* parentRenderer = static_cast<RenderSubPlanet*>(mParent->getRenderer());
        parentTexture = parentRenderer->getTexture();
    }
    mRenderer = new RenderSubPlanet(mID, mTextureTranslate, mPatchScaling, mRotation, getSektorPathName(), mPlanet->getPlanetNoiseParameters(), parentTexture);
    RenderSubPlanet* renderer = static_cast<RenderSubPlanet*>(mRenderer);
}

SubPlanetSektor::~SubPlanetSektor()
{
    GlobalRenderer::Instance().removeEbenenCount(mSubLevel);
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
    
    if(mSubLevel == 1)
    {
        DRVector3 cam = mLastRelativeCameraPosition.convertTo(KM).getVector3();
        //printf("\r cam: %f %f %f", cam.x, cam.y, cam.z);
        //printf("\ridle: %f, renderIdle: %f", mIdleSeconds, mNotRenderSeconds);
    }
    
    if(mIdleSeconds <= 0.0f)
    {
        DRVector3 camVector = mLastRelativeCameraPosition.getVector3().normalize();
        float distance = DRVector3(mLastRelativeCameraPosition/mRadius).length();
        //double angle = acos(camVector.dot(mVectorToPlanetCenter))*RADTOGRAD;
        //angle -= mPlanet->getTheta();
        
        //if(mSubLevel == 4)
            //printf("\r distance: %f", distance);
        int maxSubLevel = MAX_SUB_LEVEL;
        double relativePlayerSpeed = g_Player.getCurrentSpeed()/mRadius;
        //if(mSubLevel == 1) printf("\rplayerSpeed: %f", static_cast<double>(relativePlayerSpeed));
        if(relativePlayerSpeed <= 0.002) maxSubLevel = MAX_SUB_LEVEL;
        else if(relativePlayerSpeed <= 0.01) maxSubLevel = MAX_SUB_LEVEL-1;
        else if(relativePlayerSpeed <= 0.02) maxSubLevel = MAX_SUB_LEVEL-2;
        else if(relativePlayerSpeed <= 0.1) maxSubLevel = MAX_SUB_LEVEL-3;
        else if(relativePlayerSpeed <= 0.2) maxSubLevel = MAX_SUB_LEVEL-4;
        else if(relativePlayerSpeed <= 2) maxSubLevel = MAX_SUB_LEVEL-5;
        else maxSubLevel = MAX_SUB_LEVEL-6;
        
        //if child visible                           50 Grad                         120 Grad
        if(mSubLevel > 0 && mSubLevel < maxSubLevel &&
            mPlanet->getTheta() <= mSubLevelBorder[mSubLevel].x
            && distance < mSubLevelBorder[mSubLevel].z) /*
            //&& horizontCulling < mSubLevelBorder[mSubLevel-1].y)
            && angle < mPlanet->getTheta())
     /*   if( (mSubLevel == 1 && mPlanet->getTheta() <= 50.0 && horizontCulling < 125.0) ||
            (mSubLevel == 2 && mPlanet->getTheta() <= 30.0 && horizontCulling < 110.0f) ||
            (mSubLevel == 3 && mPlanet->getTheta() <= 15.0 && horizontCulling < 100.0f))
      * */
        {
            mNotRenderSeconds += fTime;
            s16 value = 500;
            
            //sub sektoren erstellen
            SektorID childId[4] = {SektorID( value,  value, 0),
                                   SektorID( value, -value, 0),
                                   SektorID(-value,  value, 0),
                                   SektorID(-value, -value, 0)};
            for(int i = 0; i < 4; i++)
            {
                /*DRVector3 planetVector = DRVector3(mTextureTranslate + childId[i]).transformCoords(mRotation).normalize();
                Vector3Unit posVector = (mRadius * planetVector) - (mRadius * mVectorToPlanetCenter);
                camVector = Vector3Unit(mLastRelativeCameraPosition - posVector).getVector3().normalize();
                camVector = mPlanet->getCameraPosition().getVector3().normalize();
                double angle = acos(camVector.dot(planetVector))*RADTOGRAD;
                
                //if(mSubLevel <= 3)
				angle -= 45.0f;
                //if(mSubLevel <= 5) angle -= 34.0f;
                //else angle -= 25.0f;
                
                if(angle < mPlanet->getTheta())
                {
                //if(mSubLevel >= 3 || angle <= mSubLevelBorder[mSubLevel-1].y)
                 * */
                    getChild(childId[i]);
                    //count++;
                //}
				//printf("\r angle: %f, theta: %f", angle, mPlanet->getTheta());
            }
            //if(!count) mNotRenderSeconds = 0.0f; 
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
        //removeInactiveChilds(1.0f);
    }
    
    if(render && render->getRenderNoisePlanetToTexture())
    {
        DRReal distance = static_cast<DRReal>(mLastRelativeCameraPosition.length().convertTo(M));
        if(mIdleSeconds > 0.0f) distance *= 1000.0f;
        dynamic_cast<RenderSubPlanet*>(mRenderer)->getRenderNoisePlanetToTexture()->setCurrentDistance(distance);
        //if(mNotRenderSeconds > inactiveTime)
          //  DR_SAVE_DELETE(mRenderer);
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
     
    short count = 0;
    for(std::map<u64, Sektor*>::iterator it = mChilds.begin(); it != mChilds.end(); it++)
    {
        if(it->second->isVisible())
            count++;
	}
    if(mNotRenderSeconds <= 0.0f || !count)
    {
        if(!mRenderer)
        {
            DRTexturePtr parentTexture;
            if(mSubLevel > 1 && mParent && mParent->getRenderer())
            {
                RenderSubPlanet* parentRenderer = static_cast<RenderSubPlanet*>(mParent->getRenderer());
                parentTexture = parentRenderer->getTexture();
            }
            mRenderer = new RenderSubPlanet(mID, mTextureTranslate, mPatchScaling, mRotation, getSektorPathName(), mPlanet->getPlanetNoiseParameters(), parentTexture);
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
        shader->setUniform1f("SEA_LEVEL", p->seaLevel);
        
        shader->setUniform2fv("textureCoordsParam", DRVector2(0.25f)-DRVector2(mID.x, mID.y).normalize()*DRVector2(0.25f).length());
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
    float h  = 1.0f-sqrtf(1.0f-0.5f/powf(2.0f, static_cast<float>(mSubLevel-1)));
    posInSektorNorm += mVectorToPlanetCenter*h;
    //DRVector3 sektorPosNorm = mSektorPosition.getVector3().normalize();
    float d = posInSektorNorm.dot(mVectorToPlanetCenter);
    double angle = acos(posInSektorNorm.dot(mVectorToPlanetCenter))*RADTOGRAD;   
    
   /* printf("\r angle: %f (dot: %f), idle: %f, sektorPos: %f, %f, %f, posInSektor: %f, %f, %f", angle*RADTOGRAD, d, mIdleSeconds, 
                                                                                         sektorPosNorm.x, sektorPosNorm.y, sektorPosNorm.z,
                                                                                         posInSektorNorm.x, posInSektorNorm.y, posInSektorNorm.z);
    * 
    //*/ 
    //if(mSubLevel > 1 && mPlanet->getTheta() < 45.0)
        //angle -= (45.0-mPlanet->getTheta()/2.0);
    
  //  if(mSubLevel == 2)
		//printf("\r theta: %f, angle: %f ", mPlanet->getTheta(), angle);
    
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
