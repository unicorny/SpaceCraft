#include "SubPlanetSektor.h"
#include "GlobalRenderer.h"
#include "SubPatchPlanetSektor.h"
#include "Player.h"
#include "Camera.h"
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

DRMatrix     SubPlanetSektor::mRotations[] = 
{DRMatrix::identity(),
 DRMatrix::rotationY(PI/2.0f),
 DRMatrix::rotationY(PI),
 DRMatrix::rotationY(-PI/2.0f),
 DRMatrix::rotationX(PI/2.0f),
 DRMatrix::rotationX(-PI/2.0f)};

#define MAX_SUB_LEVEL 10   // 11

int          SubPlanetSektor::mNeighborIndices[] = {2, 1, 3, 0, 0, 3, 1, 2};
int          SubPlanetSektor::mNeighborSpecialIndices[] = {3, 3, 1, 1, 2, 2, 0, 0, 
                                                           3, 0, 1, 1, 0, 1, 0, 1,
                                                           2, 1, 3, 0, 0, 3, 2, 1,
                                                           2, 3, 3, 2, 2, 2, 3, 0,
                                                           0, 3, 2, 0, 2, 2, 2, 0,
                                                           3, 1, 1, 1, 1, 3, 0, 3};

SubPlanetSektor::SubPlanetSektor(Unit radius, SektorID id, Sektor* parent, PlanetSektor* planet,
                    float patchScaling/* = 0.0f*/, int subLevel/* = 6*/)
: Sektor(Vector3Unit(0.0), radius, id, parent), mThis(NULL), mSubLevel(subLevel), mPlanet(planet),
  mPatchScaling(patchScaling), mCubeSideIndex(0), mCameraAbove(0), mVectorToPlanetCenter(DRVector3(0.0f)),
  mTextureTranslate(0.0f, 0.0f, 1.0f)
{
    mType = SUB_PLANET;  
    DRFileManager::addFolderToFileSystem(getSektorPathName().data());
    mThis = new SubPlanetSektorPtr(this);
    DRVector3 childID = mID;
    memset(mNeighbors, 0, sizeof(SubPlanetSektor*)*4);
    if(!mParent || !mPlanet)
        LOG_ERROR_VOID("no parent, no planet, nothing");
    
    GlobalRenderer::Instance().addEbenenCount(subLevel);
    
    //printf("[SubPlanetSektor::SubPlanetSektor] patchScaling: %f\n", mPatchScaling);
    if(mSubLevel == 1)
    {
        mVectorToPlanetCenter = childID.normalize();
        mSektorPosition = mRadius*mVectorToPlanetCenter;
    }
    else
    {
        SubPlanetSektor* parentSektor = dynamic_cast<SubPlanetSektor*>(mParent);
        mCubeSideIndex = parentSektor->getRotationsIndex();
        //Vector3Unit parentVector = mParent->getRadius() * parentSektor->getVectorToPlanet();
        //mVectorToPlanetCenter = (parentVector+mSektorPosition).getVector3().normalize();
        //mVectorToPlanetCenter
        if(mSubLevel > 2)
            childID = childID / powf(2.0f, static_cast<float>(mSubLevel-2));
		//printf("[SubPlanetSektor::SubPlanetSektor] id: %f %f %f\n", childID.x, childID.y, childID.z);
        //printf("[SubPlanetSektor::SubPlanetSektor] )
        mTextureTranslate = parentSektor->getTextureTranslate() + childID;
        mVectorToPlanetCenter = mTextureTranslate.transformCoords(mRotations[mCubeSideIndex]).normalize();
        
        //mTextureTranslate = parentSektor->getTextureTranslate() + childPos;
        
        mSektorPosition = (mRadius * mVectorToPlanetCenter) - (mRadius * parentSektor->getVectorToPlanet());
    }

    //berechnen der Rotationsmatrix für die Texturgenerierung, bzw. Transformation der Observer
    DRVector3 centerPosition = mVectorToPlanetCenter; 
    DRVector3 startAxis(0.00001f, 0.00001f, 1.0f);

    DRVector3 rotationAxis = startAxis.cross(centerPosition).normalize();//startAxis.cross(centerPosition).normalize();
    float rotationAngle = startAxis.dot(centerPosition);//startAxis.dot(centerPosition);

    Eigen::Affine3f affine;
    affine = Eigen::AngleAxisf(acosf(rotationAngle), Eigen::Vector3f(rotationAxis));
            //*Eigen::AngleAxisf(acosf(5.0f*GRADTORAD*faktor), Eigen::Vector3f(0.0f, 0.0f, 1.0f));

    mRotation = DRMatrix(affine.data());

    if(mSubLevel == 1)
    {
        mCubeSideIndex = static_cast<u8>(mID.count);
        mRotations[mCubeSideIndex] = mRotation;
        //printf("[SubPlanetSektor::SubPlanetSektor] center: %f %f %f\n", centerPosition.x, centerPosition.y, centerPosition.z);
    }
    /*printf("[SubPlanetSektor::SubPlanetSektor] mVectorToPlanetCenter: %f %f %f\n", mVectorToPlanetCenter.x, mVectorToPlanetCenter.y, mVectorToPlanetCenter.z);
    printf("[SubPlanetSektor::SubPlanetSektor] mTextureTranslate: %f %f %f\n", mTextureTranslate.x, mTextureTranslate.y, mTextureTranslate.z);
    mSektorPosition.print("[SubPlanetSektor::SubPlanetSektor] position");
    printf("[SubPlanetSektor::SubPlanetSektor] subLevel: %d, position.length: %s\n", mSubLevel, mSektorPosition.length().print().data());
    mParent->printTypeInfos("[SubPlanetSektor::SubPlanetSektor] parent ");
     */
 
    //mRenderer = new RenderSubPlanet(id, mTextureTranslate, patchScaling, mRotation, getSektorPathName(), mPlanet->getPlanetNoiseParameters());
    if(1 == mSubLevel)
        mRenderer = new RenderSubPlanet(mTextureTranslate, mPatchScaling, mRotations[mCubeSideIndex], getSektorPathName(), mPlanet->getPlanetNoiseParameters());    
    else if(mParent->getRenderer() && mParent->getRenderer()->isFinishLoading())
        mRenderer = new RenderSubPlanet(static_cast<RenderPlanet*>(mParent->getRenderer())->getTexture());
        
    
    //RenderSubPlanet* renderer = static_cast<RenderSubPlanet*>(mRenderer);
}

float SubPlanetSektor::calculateDistanceToNeighbor(Sektor* neighbor)
{
    if(!neighbor || !neighbor->isType(SUB_PLANET))
        return FLT_MAX;
    SubPlanetSektor* n = static_cast<SubPlanetSektor*>(neighbor);
    DRVector3 ownDir = mVectorToPlanetCenter;//.transformNormal(mRotations[mRotationsIndex]);
    DRVector3 otherDir = n->mVectorToPlanetCenter;//.transformNormal(mRotations[n->mRotationsIndex]);

    return DRVector3(ownDir - otherDir).lengthSq();
}

SubPlanetSektor::~SubPlanetSektor()
{
    mThis->removeRef();
    *mThis = NULL;
    mThis = NULL;
    GlobalRenderer::Instance().removeEbenenCount(mSubLevel);
    for(int i = 0; i < 4; i++)
    {
        if(mNeighbors[i])
        {

            if(mNeighbors[i]->getRef() <= 1)
            {
                DR_SAVE_DELETE(mNeighbors[i]);
            }
            else
            {
                mNeighbors[i]->removeRef();
            }
        }
    }
    memset(mNeighbors, 0, sizeof(SubPlanetSektor*)*4);
   // DRLog.writeToLog("SubPlanetSektor::~SubPlanetSektor");
}


DRReturn SubPlanetSektor::move(float fTime, Camera* cam)
{
    informNeighborAboutCameraAbove();
    Sektor::move(fTime, cam);
    float inactiveTime = GlobalRenderer::Instance().getTimeForInactiveChilds();
    if(mIdleSeconds)
        removeInactiveChilds(inactiveTime);
    if(!mPlanet) LOG_ERROR("planet is zero", DR_ZERO_POINTER);
    
    
    RenderSubPlanet* render = dynamic_cast<RenderSubPlanet*>(mRenderer);    
    double horizontCulling = acos(mLastRelativeCameraPosition.getVector3().normalize().dot(mVectorToPlanetCenter))*RADTOGRAD;   
    
    if(cam->getCurrentSektor() == this)// && mSubLevel == 1)
    {
        DRVector3 camDir = cam->getSektorPositionAtSektor(mPlanet).convertTo(KM).getVector3().normalize();
        // ray - plane intersection, mVectorToPlanetCenter is n and d0
        // by subplanet I must use translate
        
        DRVector3 n = PlanetSektor::mSubPlanets[mCubeSideIndex];
        // if intersection < 0, camera is on other planet surface
        float intersection = n.dot(n) /
                        camDir.dot(n);
        
        camDir = camDir.transformCoords(mRotations[mCubeSideIndex]).normalize();
        camDir *= intersection;

        //camDir -= mTextureTranslate*n;
        for(int i = 0; i < 3; i++)
        {
            if(fabs(camDir.c[i]-mTextureTranslate.c[i])>mPatchScaling) camDir.c[i] += mTextureTranslate.c[i];
            else camDir.c[i] -= mTextureTranslate.c[i];
        }
        //*/
        camDir /= mPatchScaling;
  //      camDir *= 0.5f;
    //    camDir += 0.5f;
        
       //printf("\r cam: %f %f %f, %f, %d", camDir.x, camDir.y, camDir.z, intersection, mSubLevel);
        DRVector3 camPos = cam->getSektorPositionAtSektor(mPlanet).convertTo(KM).getVector3();
        //printf("\r [SubPlanetSektor::Move] cam: %f %f %f = %f", camPos.x, camPos.y, camPos.z, camPos.length());
    }
    
    if(mSubLevel > 1 && !mPlanet->isReady()) return DR_OK;
    
    if(mRenderer && mRenderer->isErrorOccured())
        DR_SAVE_DELETE(mRenderer);
    if(!mRenderer)
    {
        if(1 == mSubLevel)
            mRenderer = new RenderSubPlanet(mTextureTranslate, mPatchScaling, mRotations[mCubeSideIndex], getSektorPathName(), mPlanet->getPlanetNoiseParameters());
        else if(mParent->getRenderer() && mParent->getRenderer()->isFinishLoading())
            mRenderer = new RenderSubPlanet(static_cast<RenderPlanet*>(mParent->getRenderer())->getTexture());
    }
    if(!mRenderer) 
        return DR_OK;//LOG_ERROR("no renderer", DR_ERROR);
    RenderSubPlanet* renderer = static_cast<RenderSubPlanet*>(mRenderer);
    if(!renderer->isInitalized() && isSektorVisibleToRender())
    {
        int size = GlobalRenderer::Instance().getTextureRenderMaxResolution();
        renderer->init(mTextureTranslate, mPatchScaling, mRotations[mCubeSideIndex], size, getSektorPathName(), mPlanet->getPlanetNoiseParameters());
    }
    // set ready count bit 2^sektorNummer
    if(!renderer->isFinishLoading())
    {
        renderer->generateTexture();       
        return DR_OK;
    }
    else if(mSubLevel == 1)
    {
        mPlanet->setReadyCount(static_cast<u8>(powf(2.0f, static_cast<float>(mID.count))));
        if(!mPlanet->isReady()) return DR_OK;
    }

    if(mIdleSeconds <= 0.0f)
    {
     //   DRVector3 camVector = mLastRelativeCameraPosition.getVector3().normalize();
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
        
        maxSubLevel = MAX_SUB_LEVEL;
        
        // new Conditions:
        // getChild if (camera is above this sector AND theta 
        //              AND Kamera high above heighest point AND max gradient)
        
        // if child visible and renderer is finished, only than create childs
        //if child visible                           50 Grad                         120 Grad
        if(mSubLevel > 0 && mSubLevel < maxSubLevel &&
            mPlanet->getTheta() <= mSubLevelBorder[mSubLevel].x
            && isSektorVisibleToRender() && mRenderer && mRenderer->isFinishLoading()) /*
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
            SektorID childId[4] = {SektorID( value,  value, 0, 0),
                                   SektorID( value, -value, 0, 1),
                                   SektorID(-value,  value, 0, 2),
                                   SektorID(-value, -value, 0, 3)};
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
            //Set neighbor pointer
        //    if(mSubLevel > 1)
            {
                // binär: 1001 1100 0011 0110;
                u16 bitMask = 1+8+16+32+1024+2048+8192+16384;
                // iChild: 0, i = 0, i = 3
                // iChild: 1, i = 0, i = 1
                // iChild: 2, i = 2, i = 3
                // iChild: 3, i = 1, i = 2
                for(u8 iChild = 0; iChild < 4; iChild++)
                {
                    if(mChilds.find(childId[iChild]) == mChilds.end()) 
                        LOG_ERROR("a subPlanetChild is missing", DR_ERROR);
                    SubPlanetSektor* child = static_cast<SubPlanetSektor*>(mChilds[childId[iChild]]);
                    u8 specialCursor = 0;
                    for(u8 i = 0; i < 4; i++)
                    {
                        u8 cursor = iChild*2+i;
                        if(i > 1) cursor-=2;
                        if(bitMask & static_cast<int>(powf(2.0f, static_cast<float>(iChild*4+i))))
                        {
                            Sektor* t = NULL;
                            
                            u8 neighborIndex = mNeighborIndices[cursor];
                            u8 neighborThisIndex = 0;
                            SubPlanetSektor* neighbor = NULL;
                            if(getNeighbor(i)&& *getNeighbor(i))
                                neighbor = *getNeighbor(i);
                            if(neighbor)
                            {
                                for(u8 iNeighbor = 0; iNeighbor < 4; iNeighbor++ )
                                    if(neighbor->getNeighbor(iNeighbor) && this == *neighbor->getNeighbor(iNeighbor))
                                        neighborThisIndex = iNeighbor;
                                if(i-neighborThisIndex != 2 && neighborThisIndex-i != 2)
                                    neighborIndex = mNeighborSpecialIndices[mCubeSideIndex*8+iChild*2+specialCursor];
                                if(neighbor->isChildExist(childId[neighborIndex]))
                                    t = neighbor->getChild(childId[neighborIndex]);
                            }      
                            child->setNeighbor(i, t);
                            specialCursor++;
                        }
                        else
                        {
                            child->setNeighbor(i, getChild(childId[mNeighborIndices[cursor]]));
                        }
                    }
                }
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
        
        //removeInactiveChilds(1.0f);
    }
    
    
    
    return DR_OK;
}
void SubPlanetSektor::informNeighborAboutCameraAbove()
{
    if(isCameraAbove())
    {
        u8 cameraAboveConfig[] = {CAMERA_ABOVE_SECTOR_SIDE_LEFT,
                                  CAMERA_ABOVE_SECTOR_SIDE_TOP,
                                  CAMERA_ABOVE_SECTOR_SIDE_RIGHT,
                                  CAMERA_ABOVE_SECTOR_SIDE_BOTTOM};
        //tell neighbors in which edge we are
        for(u8 i = 0; i < 4; i++)
        {
            if(mNeighbors[i] && *mNeighbors[i])
            {
                if(cameraAboveConfig[i] & mCameraAbove)
                    (*mNeighbors[i])->setCameraAboveNeighbor();
            }
        }
    }
}

void SubPlanetSektor::setCameraAboveNeighbor()
{
    mCameraAbove |= CAMERA_IS_ABOVE_NEIGHBOR_SECTOR; 
    for(int i = 0; i < 4; i++)
    {
        if(mNeighbors[i] && *mNeighbors[i])
            (*mNeighbors[i])->setCameraIsMaybeAboveNeigbhorEdge();
    }
}

DRReturn SubPlanetSektor::updateVisibility(const std::list<Camera*>& cameras)
{
    RenderSubPlanet* render = dynamic_cast<RenderSubPlanet*>(mRenderer);
    DRReal distance = 1.0e18f;
    
    if(render && render->getRenderNoisePlanetToTexture())
    {
        if(1 == mSubLevel)
            render->getRenderNoisePlanetToTexture()->setCurrentDistance(1);
        if(isSektorVisibleToRender())
            render->getRenderNoisePlanetToTexture()->setCurrentDistance(mSubLevel *10 - (mCameraAbove & 1)*mSubLevel *9 - (mCameraAbove & 6? 2 : 0));
        else
            render->getRenderNoisePlanetToTexture()->setCurrentDistance(distance);
    }
    //mCameraAbove = 0;
    u8 cameraAbove = mCameraAbove;
    
    bool currentCameraAbove = false;
    for(std::list<Camera*>::const_iterator it = cameras.begin(); it != cameras.end(); it++)
    {
        Vector3Unit pos = (*it)->getSektorPositionAtSektor(this);
        DRReal local_distance = static_cast<DRReal>(pos.length().convertTo(M));
        if(local_distance < distance) distance = local_distance;
        
        // is camera above sector
        if(isObjectInSektor(*it))
            currentCameraAbove = true;
    }
    if(!(cameraAbove & CAMERA_IS_ABOVE_SECTOR) && currentCameraAbove)
        mCameraAbove |= CAMERA_IS_ABOVE_SECTOR;
    else if(cameraAbove & CAMERA_IS_ABOVE_SECTOR && !currentCameraAbove)
        resetCameraAbove();
    else if(cameraAbove & CAMERA_IS_ABOVE_SECTOR && currentCameraAbove &&
            (cameraAbove & CAMERA_ABOVE_SECTOR_EDGE) != (mCameraAbove & CAMERA_ABOVE_SECTOR_EDGE))
        resetCameraAbove();
    
    if(render && render->getRenderNoisePlanetToTexture())
    {
        if(mIdleSeconds > 0.0f) distance *= 1000.0f;
        //render->getRenderNoisePlanetToTexture()->setCurrentDistance(distance);
        //if(mNotRenderSeconds > inactiveTime)
          //  DR_SAVE_DELETE(mRenderer);
    }
    return DR_OK;
}

void SubPlanetSektor::resetCameraAbove(int deep /* = 0*/)
{
    mCameraAbove = 0;
    if(deep < 3)
    {
        for(int i = 0; i < 4; i++)
        {
            if(mNeighbors[i]&& *mNeighbors[i])
                (*mNeighbors[i])->resetCameraAbove(deep+1);
        }
    }
}

DRReturn SubPlanetSektor::render(float fTime, Camera* cam)
{
    //if(mIdleSeconds > 0.0f) return DR_NOT_ERROR;
    
    //DRVector3 childPos(mID.x, mID.y, mID.z);
    //childPos /= 1000.0f;
    if(mSubLevel == 1)
        mMatrix = mRotation * mParent->getMatrix();
    else
        mMatrix = mParent->getMatrix();
    
    //new Matrix
    double alpha = (M_PI/2.0)/pow(2.0,(static_cast<double>(mSubLevel)-1.0));
    double r = mRadius.convertTo(KM);
    double R = sqrt(r*r-r*r*cos(alpha));
    DRVector3 transformVertices = DRVector3(0.0f);
    
    if(cam->getCurrentSektor() == this)
        printf("\r %d", mSubLevel);
    
    static float rotate = 0.0;
    //if(cam->getCurrentSektor() == this)
       //rotate += .2*fTime;
   // if(cam->getCurrentSektor() == this)
     //   printf("\r[SubPlanetSektor::render] R:%f, rotate: %f", R, rotate);
   if(mSubLevel > 4)// && cam->getCurrentSektor() == this)
    {    
        DRVector3 centerPosition = mTextureTranslate.normalize(); 
        DRVector3 startAxis(0.00001f, 0.00001f, 1.0f);

        DRVector3 rotationAxis = startAxis.cross(centerPosition).normalize();//startAxis.cross(centerPosition).normalize();
        float rotationAngle = startAxis.dot(centerPosition);//startAxis.dot(centerPosition);
        
        Eigen::Affine3f affine;
        affine = Eigen::AngleAxisf(acosf(rotationAngle), Eigen::Vector3f(rotationAxis));
            //*Eigen::AngleAxisf(acosf(5.0f*GRADTORAD*faktor), Eigen::Vector3f(0.0f, 0.0f, 1.0f));

        transformVertices = DRVector3(0.0, 0.0, sqrt(cos(alpha))).transformCoords(DRMatrix(affine.data()));
    
        DRVector3 camPos = -mLastRelativeCameraPosition.convertTo(KM).getVector3();
        
        //printf("\r [SubPlanetSektor::Render] cam: %f km %f km %f km = %f", camPos.x, camPos.y, camPos.z, camPos.length());
        
        
        //camPos = -camPos;//DRVector3(0.0f, -1.0f, 0.0f);
        DRVector3 scale = DRVector3(static_cast<DRReal>(R/(mPatchScaling*1.004f)));
        scale = DRVector3( mRadius.convertTo(KM)*1.0001);
        //scale = DRVector3(1.0);

//        mMatrix = DRMatrix::scaling(DRVector3(static_cast<DRReal>(R*0.5))) * DRMatrix::rotationZ(rotate) * mRotation * DRMatrix::translation(-mLastRelativeCameraPosition.convertTo(KM).getVector3()) * cam->getCameraMatrixRotation();
        mMatrix = DRMatrix::scaling(scale) * mRotations[mCubeSideIndex] * DRMatrix::translation(camPos)  * cam->getCameraMatrixRotation();
        
    }
    
    short count = 0;
    for(std::map<u64, Sektor*>::iterator it = mChilds.begin(); it != mChilds.end(); it++)
    {
        if(it->second->isVisible())
            count++;
	}
 
    if(mNotRenderSeconds <= 0.0f || !count)
    {
       /* if(static_cast<RenderSubPlanet*>(mRenderer)->isErrorOccured())
        {
            LOG_WARNING("Renderer deleted, because an error!");
            DR_SAVE_DELETE(mRenderer);
        }
        if(!mRenderer)
            mRenderer = new RenderSubPlanet(mTextureTranslate, mPatchScaling, mRotations[mCubeSideIndex], getSektorPathName(), mPlanet->getPlanetNoiseParameters());
        * */
        if(!mRenderer) 
            LOG_ERROR("no renderer", DR_NOT_ERROR);
        DRVector3 translate = mTextureTranslate;
        float scale = mPatchScaling*1.004f;
        

        ShaderProgramPtr shader = mRenderer->getShaderProgram();
        const PlanetNoiseParameter* p = mPlanet->getPlanetNoiseParameters();
        if(!shader) LOG_ERROR("renderer shader isn't valid", DR_ERROR);
        shader->bind();
        shader->setUniformMatrix("projection", GlobalRenderer::Instance().getProjectionMatrix());
        shader->setUniformMatrix("modelview", mMatrix);
        shader->setUniform3fv("translate", translate);
        shader->setUniform3fv("transformVertices", transformVertices);
        shader->setUniform1f("patchScaling", scale);
        shader->setUniform1f("MAX_HEIGHT_IN_PERCENT", p->maxHeightInPercent);
        shader->setUniform1f("MIN_HEIGHT_IN_PERCENT", p->minHeightInPercent);
        shader->setUniform1f("SEA_LEVEL", p->seaLevel);
        shader->setUniform1i("cameraAbove", static_cast<int>(mCameraAbove & 7));
        
       if(!mRenderer->isFinishLoading())
            shader->setUniform2fv("textureCoordsParam", DRVector2(0.25f)-DRVector2(mID.x, mID.y).normalize()*DRVector2(0.25f).length());
        else
            shader->setUniform2fv("textureCoordsParam", DRVector2(0.0f));
        mRenderer->render(fTime, cam);
        shader->unbind();
        //GlobalRenderer::getSingleton().getPlanetShaderPtr()->unbind();
        // childs didn't need to render
        return DR_NOT_ERROR;
    }
    return DR_OK;
}

bool SubPlanetSektor::isObjectInSektor(SektorObject* sektorObject)
{    
    if(mSubLevel > 0 &&
       mPlanet->getTheta() > mSubLevelBorder[mSubLevel-1].x) return false;
        
    Vector3Unit positionOnPlanet = sektorObject->getSektorPositionAtSektor(mPlanet);    

    DRVector3 camDir = positionOnPlanet.convertTo(KM).getVector3().normalize();
    DRVector3 n = PlanetSektor::mSubPlanets[mCubeSideIndex];
    // if intersection < 0, camera is on other planet surface
    float intersection = n.dot(n) /
                    camDir.dot(n);
    if(intersection < 0) return false;

    camDir = camDir.transformCoords(mRotations[mCubeSideIndex].invert()).normalize();
    camDir *= intersection;

    for(int i = 0; i < 2; i++)
    {
        if(fabs(camDir.c[i]-mTextureTranslate.c[i]) > mPatchScaling)
            return false;
    }
    camDir -= mTextureTranslate;
    camDir /= mPatchScaling;
    int flag = 0;
    if(camDir.x > 0.0f && camDir.y < 0.0f)
        flag = CAMERA_ABOVE_SECTOR_EDGE_TOP_LEFT;
    else if(camDir.x > 0.0f && camDir.y > 0.0f)
        flag = CAMERA_ABOVE_SECTOR_EDGE_BOTTOM_LEFT;
    else if(camDir.x < 0.0f && camDir.y > 0.0f)
        flag = CAMERA_ABOVE_SECTOR_EDGE_BOTTOM_RIGHT;
    else if(camDir.x < 0.0f && camDir.y < 0.0f)
        flag = CAMERA_ABOVE_SECTOR_EDGE_TOP_RIGHT;
    else
        LOG_ERROR("not expected", false);
    mCameraAbove |= flag;
    
    if(sektorObject->getCurrentSektor() == this)
    {
     //  printf("\r cam: %f %f %f, %f, %d, camAbove: %d", camDir.x, camDir.y, camDir.z, intersection, mSubLevel, mCameraAbove);
    }
        
    return true;  
}

bool SubPlanetSektor::isSectorVisibleFromPosition(Vector3Unit positionInSektor)
{
    if(mSubLevel > 0 &&
       mPlanet->getTheta() >= mSubLevelBorder[mSubLevel-1].x)
        return false;
    
    DRVector3 posInSektorNorm = positionInSektor.getVector3().normalize();
    float h  = 1.0f-sqrtf(1.0f-0.5f/powf(2.0f, static_cast<float>(mSubLevel-1)));
    posInSektorNorm += mVectorToPlanetCenter*h;
    //DRVector3 sektorPosNorm = mSektorPosition.getVector3().normalize();
    float d = posInSektorNorm.dot(mVectorToPlanetCenter);
    double angle = acos(posInSektorNorm.dot(mVectorToPlanetCenter))*RADTOGRAD;   
    //if(angle > mSubLevelBorder[mSubLevel-1].y) return false;
    if(!isSektorVisibleToRender()) return false;
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
        
    }
    //*/
    return mChilds[childID];
}

void SubPlanetSektor::printTypeInfos(const char* name) const
{
    printf("%s SubPlanetSektor, subLevel: %d, id: %d %d %d\n", name, mSubLevel, mID.x, mID.y, mID.z);
}

DRString SubPlanetSektor::getSektorPathName()
{
    std::stringstream s(std::stringstream::in|std::stringstream::out);

    const std::vector<SektorID> path = getSektorPath();  
    SektorID sektorID = path[0];
    
    for(uint i = 0; i < path.size(); i++)
    {
        sektorID = path[i];
        if(0 == i) {
            s << "./data/_" << sektorID << "/"; }
        else if(i < 3) {
            s << "_" << sektorID << "/"; }
        else {
            s << "_" << sektorID.x << sektorID.y << sektorID.z << "/"; 
        }
    }
    return s.str();
}