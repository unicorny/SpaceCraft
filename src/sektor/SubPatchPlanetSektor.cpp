#include "SubPatchPlanetSektor.h"
#include "GlobalRenderer.h"

SubPatchPlanetSektor::SubPatchPlanetSektor(Vector3Unit position, Unit radius, 
                                           SektorID id, Sektor* parent, PlanetSektor* planet,
                                           float cameraDistance/* = 0.0f*/, int subLevel/* = 7*/)
: SubPlanetSektor(position, radius, id, parent, planet, cameraDistance, subLevel)
{
    mType = SUB_PATCH_PLANET;  
    
    mRotation = dynamic_cast<SubPlanetSektor*>(mParent)->getRotation();
    mRenderer = new RenderSubPatchPlanet(id, mTheta, cameraDistance, mRotation, getSektorPathName());
}

 SubPatchPlanetSektor::~SubPatchPlanetSektor()
 {
     //DRLog.writeToLog("deconstruct SubPatchPlanetSektor");
 }

 DRReturn SubPatchPlanetSektor::move(float fTime, Camera* cam)
 {
    mHorizontCulling = 75.0;
    if(mSubLevel != 7) return DR_OK;
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
        /*///sub sektoren erstellen
        getChild(SektorID(-500, -500, 0)); // rechts unten
        getChild(SektorID( 500, -500, 0)); // links unten
        getChild(SektorID(-500,  500, 0)); //rechts oben
        getChild(SektorID(-500, -500, 0)); //rechts unten
    //* */
    }
    else
    {
        removeInactiveChilds(60.0f);
    }
    return DR_OK;
 }
    
   
 DRReturn SubPatchPlanetSektor::render(float fTime, Camera* cam)
 {
     //if(mIdleSeconds > 0.0f) return DR_OK;
    //DRVector3 pos = mSektorPosition.getVector3().normalize();
    
    //glMultMatrixf(mRotation);
    //glTranslatef(0.0f, 0.0f, 1.0f-mEbeneDistanceToCenter);
    DRVector3 translate = DRVector3(0.0f, 0.0f, 1.0f-mEbeneDistanceToCenter);
     DRVector3 childPos(mID.x, mID.y, mID.z);
     childPos /= 1000.0f;
    mMatrix = DRMatrix::translation(translate) * mRotation * mPlanet->getMatrix();//  mParent->getMatrix();
    ShaderProgram* shader = mRenderer->getShaderProgram();
    if(!shader) LOG_ERROR("renderer shader isn't valid", DR_ERROR);
    shader->bind();
    shader->setUniformMatrix("projection", GlobalRenderer::Instance().getProjectionMatrix());
    shader->setUniformMatrix("modelview", mMatrix);
	

    //if(mHorizontCulling > 70.0)
    {
        shader->setUniform1f("theta", mTheta);
        shader->setUniform1f("cameraDistance", mEbeneDistanceToCenter);
        DRVector3 sp = mSphericalCenter;
        sp = DRVector3(0.0f, 0.0f, -1.0f*(1.0f-mEbeneDistanceToCenter));
        shader->setUniform3fv("SphericalCenter", sp);//DRVector3(0.0f, 0.0f, -1.0f*(1.0f-mEbeneDistanceToCenter)));
        mRenderer->render(fTime, cam);
        shader->unbind();
        //GlobalRenderer::getSingleton().getPlanetShaderPtr()->unbind();
        // childs didn't need to render
        return DR_NOT_ERROR;
    }
    return DR_OK;
 }