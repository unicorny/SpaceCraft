#include "main.h"

SubPlanetSektor::SubPlanetSektor(Vector3Unit position, Unit radius, SektorID id, Sektor* parent, PlanetSektor* planet,
                    float cameraDistance/* = 0.0f*/, int subLevel/* = 6*/)
: Sektor(position, radius, id, parent), mSubLevel(subLevel), mHorizontCulling(0.0), mPlanet(planet), mTheta(acos(cameraDistance)),
  mEbeneDistanceToCenter(1.0f-cameraDistance)
{
    mType = SUB_PLANET;  
    memset(mNeighbors, 0, sizeof(SubPlanetSektor*)*4);

    //berechnen der Rotationsmatrix für die Texturgenerierung
    DRVector3 centerPosition = position.getVector3().normalize(); 
    DRVector3 startAxis(0.0001f, 0.0001f, 1.0000f);
   
    DRVector3 rotationAxis = startAxis.cross(centerPosition).normalize();
    float rotationAngle = startAxis.dot(centerPosition);

    Eigen::Affine3f affine;
    affine = Eigen::AngleAxisf(acosf(rotationAngle), Eigen::Vector3f(rotationAxis));

    mRotation = DRMatrix(affine.data());
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
   /*     getChild(SektorID(-1, 1, 0));  // rechts unten
        getChild(SektorID( 1, 1, 0));  // links unten
        getChild(SektorID( 1,-1, 0));
        getChild(SektorID(-1,-1, 0));
    * */
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
	

    //GlobalRenderer::getSingleton().getPlanetShaderPtr()->bind();
    //if(mHorizontCulling > 70.0)
    //{
		shader->setUniform1f("theta", mTheta);
		shader->setUniform3fv("SphericalCenter", DRVector3(0.0f, 0.0f, -1.0f*(1.0f-mEbeneDistanceToCenter)));
        mRenderer->render(fTime, cam);
		shader->unbind();
        //GlobalRenderer::getSingleton().getPlanetShaderPtr()->unbind();
        // childs didn't need to render
        return DR_NOT_ERROR;
    //}
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
        
        DRVector3 center = DRVector3(0.0f, 0.0f, 1.0);
        
        DRVector3 dir = DRVector3(center+mSektorPosition.getVector3().normalize());
        float length = dir.length();
        DRVector3 centerNormalized = center +  (dir / length) * (1.0f-length);
        
        // Position des Quadratmittelpunktes
        Vector3Unit position = Vector3Unit(centerNormalized*mRadius)+mSektorPosition;
        position = position.convertTo(KM);
        position.print("planet pos"); printf("subLevel: %d, %s\n", mSubLevel, position.length().print().data());

        Unit radius = mRadius;// * faktorH;
        //printf("radius: %s\n", radius.print().data());
        SubPlanetSektor* temp = new SubPlanetSektor(position, radius, childID, this, mPlanet, mSubLevel + 1);
        mChilds.insert(SEKTOR_ENTRY(childID, temp));

        //Set neighbor pointer
        for(int i = 0; i < 4; i++)
        {
            
        }
    }
    //*/
    return mChilds[childID];
}

