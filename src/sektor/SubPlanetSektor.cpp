#include "SubPlanetSektor.h"
#include "GlobalRenderer.h"
#include "SubPatchPlanetSektor.h"

SubPlanetSektor::SubPlanetSektor(Vector3Unit position, Unit radius, SektorID id, Sektor* parent, PlanetSektor* planet,
                    float cameraDistance/* = 0.0f*/, int subLevel/* = 6*/)
: Sektor(position, radius, id, parent), mSubLevel(subLevel), mHorizontCulling(0.0), mPlanet(planet), mTheta(acos(cameraDistance)),
  mEbeneDistanceToCenter(1.0f-cameraDistance), mSphericalCenter(DRVector3(0.0f))
{
    mType = SUB_PLANET;  
    memset(mNeighbors, 0, sizeof(SubPlanetSektor*)*4);
    if(subLevel > 6) return;
    
    //berechnen der Rotationsmatrix für die Texturgenerierung
    DRVector3 centerPosition = position.getVector3().normalize(); 
    DRVector3 startAxis(0.00001f, 0.00001f, 1.0f);
    mSphericalCenter = DRVector3(0.5f, 0.5f, -0.5f).normalize() *cameraDistance;
   
    DRVector3 rotationAxis = startAxis.cross(centerPosition).normalize();//startAxis.cross(centerPosition).normalize();
    float rotationAngle = startAxis.dot(centerPosition);//startAxis.dot(centerPosition);

    Eigen::Affine3f affine;
    affine = Eigen::AngleAxisf(acosf(rotationAngle), Eigen::Vector3f(rotationAxis));
            //*Eigen::AngleAxisf(acosf(5.0f*GRADTORAD*faktor), Eigen::Vector3f(0.0f, 0.0f, 1.0f));

    mRotation = DRMatrix(affine.data());
    //mRotation = DRMatrix::identity();
    //mSphericalCenter = mSphericalCenter.transformCoords(mRotation.invert() );
    printf("center: %f %f %f, spherical: %f %f %f\n", centerPosition.x, centerPosition.y, centerPosition.z, mSphericalCenter.x, mSphericalCenter.y, mSphericalCenter.z);
    
    mRenderer = new RenderSubPlanet(id, mTheta, cameraDistance, mRotation, getSektorPathName());
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
        short value = 310;
        //sub sektoren erstellen
        getChild(SektorID(-value, -value, 0)); // rechts unten
        getChild(SektorID( value, -value, 0)); // links unten
        getChild(SektorID(-value,  value, 0)); //rechts oben
        getChild(SektorID(-value, -value, 0)); //rechts unten
    //* */
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
    //DRVector3 pos = mSektorPosition.getVector3().normalize();
    
    //glMultMatrixf(mRotation);
    //glTranslatef(0.0f, 0.0f, 1.0f-mEbeneDistanceToCenter);
    DRVector3 translate = DRVector3(0.0f, 0.0f, 1.0f-mEbeneDistanceToCenter);
    mMatrix = DRMatrix::translation(translate) * mRotation * mParent->getMatrix();
    ShaderProgram* shader = mRenderer->getShaderProgram();
    if(!shader) LOG_ERROR("renderer shader isn't valid", DR_ERROR);
    shader->bind();
    shader->setUniformMatrix("projection", GlobalRenderer::Instance().getProjectionMatrix());
    shader->setUniformMatrix("modelview", mMatrix);
	

    if(mHorizontCulling > 70.0)
    {
        shader->setUniform1f("theta", mTheta);
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
        
        // Position des Quadratmittelpunktes
        DRVector3 childPos(childID.x, childID.y, childID.z);
        childPos /= 1000.0f;
        Vector3Unit position = Vector3Unit(childPos*mRadius);
        position = position.convertTo(KM);
        position.print("planet pos"); printf("subLevel: %d, %s\n", mSubLevel+1, position.length().print().data());

        Unit radius = mRadius;// * faktorH;
        //printf("radius: %s\n", radius.print().data());
        //906 ist zu groß (lücken links und rechts)
        SubPlanetSektor* temp = new SubPatchPlanetSektor(position, radius, childID, this, mPlanet, 0.905f, mSubLevel + 1);
        mChilds.insert(SEKTOR_ENTRY(childID, temp));

        //Set neighbor pointer
        for(int i = 0; i < 4; i++)
        {
            
        }
    }
    //*/
    return mChilds[childID];
}

