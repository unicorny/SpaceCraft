#include "main.h"

SektorID PlanetSektor::mSubPlanets[] = {SektorID(0,0,-1),SektorID(1,0,0),SektorID(0,0, 1),// front, right, back
                                        SektorID(-1,0,0),SektorID(0,1,0),SektorID(0,-1,0)};// left, top, bottom

PlanetSektor::PlanetSektor(Vector3Unit position, Unit radius, SektorID id, Sektor* parent)
: Sektor(position, radius, id, parent)
{
    mType = PLANET;
    
    mNoiseGenerator = new GenerateNoisePlanet();
    mHeights = new PlanetHeightValues(mNoiseGenerator);
        
	noise::module::Perlin p;
	if(id.count) p.SetSeed(id.count);
	DRVector3 idVector(id.x, id.y, id.z);
	idVector /= SHRT_MAX;
    mNoiseGenerator->setupGenerator((int)(p.GetValue(idVector.x, idVector.y, idVector.z)*INT_MAX));
    
    double seaLevelInMeters = mNoiseGenerator->getSeaLevelInMetres();
    
    mHeights->ClearGradientPoints ();
    mHeights->AddGradientPoint (-2.0 + seaLevelInMeters, noise::utils::Color (  0,   0,   0, 255));
    mHeights->AddGradientPoint (    -0.03125 + seaLevelInMeters, noise::utils::Color (  6,  58, 127, 255));
    mHeights->AddGradientPoint (    -0.0001220703 + seaLevelInMeters, noise::utils::Color ( 14, 112, 192, 255));
    mHeights->AddGradientPoint (     0.0 + seaLevelInMeters, noise::utils::Color ( 70, 120,  60, 255));
    mHeights->AddGradientPoint (  0.125 + seaLevelInMeters, noise::utils::Color (110, 140,  75, 255));
    mHeights->AddGradientPoint (  0.25 + seaLevelInMeters, noise::utils::Color (160, 140, 111, 255));
    mHeights->AddGradientPoint (  0.375 + seaLevelInMeters, noise::utils::Color (184, 163, 141, 255));
    mHeights->AddGradientPoint (  0.5 + seaLevelInMeters, noise::utils::Color (255, 255, 255, 255));
    mHeights->AddGradientPoint (  0.75 + seaLevelInMeters, noise::utils::Color (128, 255, 255, 255));
    mHeights->AddGradientPoint ( 2.0 + seaLevelInMeters, noise::utils::Color (  0,   0, 255, 255));
    
    mRenderer = new RenderPlanet(id);
}

PlanetSektor::~PlanetSektor()
{
    DR_SAVE_DELETE(mRenderer);
    DR_SAVE_DELETE(mHeights);
    DR_SAVE_DELETE(mNoiseGenerator);
}

DRReturn PlanetSektor::move(float fTime, Camera* cam)
{
    mLastRelativeCameraPosition = cam->getSektorPositionAtSektor(this);
    double horizontAngle = acos(mRadius/mLastRelativeCameraPosition.length());
    Unit distance = mLastRelativeCameraPosition.length()-mRadius;
    distance = distance.convertTo(KM);
    //printf("\rEntfernung zur Oberflaeche: %s", distance.print().data());
    
    if(isObjectInSektor(mLastRelativeCameraPosition))
    {                
        for(u32 i = 0; i < 6; i++)
        {
            //horizont culling
            DRVector3 camPos = mLastRelativeCameraPosition.getVector3().normalize();
            double angle = acos(camPos.dot(DRVector3(mSubPlanets[i].x, mSubPlanets[i].y, mSubPlanets[i].z)))-PI/4.0;            
            if(angle < horizontAngle)
                getChild(mSubPlanets[i]);            
            //else
                //printf("\r %d, angle: %f, horizontAngle: %f", i, angle*RADTOGRAD, horizontAngle*RADTOGRAD);
        }
    }
    else
    {
        //removeInactiveChilds(1.0f);
    }
    removeInactiveChilds(60.0f);
    return DR_OK;
}

DRReturn PlanetSektor::render(float fTime, Camera* cam)
{
    //if(isObjectInSektor(cam->getSektorPosition())) return DR_OK;
	//Unit distance1 = Vector3Unit(mSektorPosition - cam->getSektorPosition()).length();
    //Unit distance1 = Vector3Unit(mSektorPosition - mLastRelativeCameraPosition).length();    
    Unit distance1 = (-mLastRelativeCameraPosition).length();    
	//DRVector3 diff = Vector3Unit(mSektorPosition - cam->getSektorPosition()).convertTo(KM).getVector3();

	distance1 = distance1.convertTo(mRadius.getType());
	double distance2 = 200.0f;
	Unit radius1 = mRadius;
	double radius2 = ((radius1 * distance2) / distance1);
    
	if(radius2 > 160.0f) mRenderer->setCurrentDetail(10);
	else if(radius2 > 140.0f) mRenderer->setCurrentDetail(9);
	else if(radius2 > 120.0f) mRenderer->setCurrentDetail(8);
	else if(radius2 > 90.0f) mRenderer->setCurrentDetail(7);
	else if(radius2 > 70.0f) mRenderer->setCurrentDetail(6);
	else if(radius2 > 30.0f) mRenderer->setCurrentDetail(5);
	else if(radius2 > 25.0f) mRenderer->setCurrentDetail(4);
	else if(radius2 > 15.0f) mRenderer->setCurrentDetail(3);
	else if(radius2 > 5.0f) mRenderer->setCurrentDetail(2);
	else if(radius2 > 1.0f) mRenderer->setCurrentDetail(1);
	else mRenderer->setCurrentDetail(0);

	//DRVector3 pos = (mSektorPosition - cam->getSektorPosition()).getVector3().normalize();
    //DRVector3 pos = (mSektorPosition - mLastRelativeCameraPosition).getVector3().normalize();
    DRVector3 pos = (-mLastRelativeCameraPosition).getVector3().normalize();
	DRVector3 relCameraPos = -pos*distance1/mRadius;
	pos *= distance2;
/*   printf("\r %f %f %f, %.8f, %s  x:%s y:%s z:%s (%f %f %f)", pos.x, pos.y, pos.z, radius2, distance1.print().data(),
									   absCameraPosition.x.print().data(), absCameraPosition.y.print().data(),
									   absCameraPosition.z.print().data(), diff.x, diff.y, diff.z);
										   //*/


	glTranslatef(pos.x, pos.y, pos.z);
    glScaled(radius2, radius2, radius2);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//if(mRenderer && !isObjectInSektor(cam->getSektorPosition()))
    if(mRenderer && !isObjectInSektor(mLastRelativeCameraPosition))
	{
      //GlobalRenderer::getSingleton().getPlanetShaderPtr()->bind();
        //glPushMatrix();
        
		DRReturn ret = mRenderer->render(fTime, cam);
		GlobalRenderer::getSingleton().getPlanetShaderPtr()->unbind();
        //glPopMatrix();
		if(ret) LOG_ERROR("Fehler bei call planet renderer", DR_ERROR);
        //child didn't need to render
        return DR_NOT_ERROR;
	}    
    
    return DR_OK;
}

Sektor* PlanetSektor::getChild(SektorID childID)
{        
    if(mChilds.find(childID) == mChilds.end())
    {
        // subPlanet seiten eines Würfels mit Kantenlänge 2 (intern)
        // Kantenpunktlänge = Wurzel(3), radius = 1, Quadratmittelpunktlänge = 1
        // Wurzel(3)       1
        //  ------    =  -----   => Wurzel(3) = 1/faktor => faktor = 1/Wurzel(3)
        //     1         faktor
        double faktor = 1.0/sqrt(3.0);// sqrtf(3.0) = ca. 1.73205, faktor = ca. 0.577
        double faktorH = sqrt(2.0/sqrt(3.0)-1.0/3.0);

        // Position des Quadratmittelpunktes
        //Vector3Unit position = Vector3Unit(childID.x, childID.y, childID.z, KM)*mRadius*faktor;
        Vector3Unit position = Vector3Unit(childID.x, childID.y, childID.z, KM)*mRadius;//*faktor;
        //position.print("planet pos");

        Unit radius = mRadius * faktorH;
        printf("radius: %s\n", radius.print().data());
        SubPlanetSektor* temp = new SubPlanetSektor(position, radius, childID, this, this, NULL);
        mChilds.insert(SEKTOR_ENTRY(childID, temp));

        //Set neighbor pointer
        for(int i = 0; i < 6; i++)
        {
            SubPlanetSektor* n = NULL;
            if(i >= 1 && i <= 3) // right, back, left
            {
                if(mChilds.find(mSubPlanets[i-1]) != mChilds.end())
                {
                    n = dynamic_cast<SubPlanetSektor*>(mChilds[mSubPlanets[i-1]]);
                    if(temp == n) continue;
                    temp->setNeighbor(NEIGHBOR_LEFT, n);//left                    
                    n->setNeighbor(NEIGHBOR_RIGHT, temp);//right 
                }
            }
            else if(4 == i || 5 == i)//top, bottom
            {
                for(u32 j = 0; j < 4; j++)
                {
                    if(mChilds.find(mSubPlanets[j]) != mChilds.end())
                    {
                        n = dynamic_cast<SubPlanetSektor*>(mChilds[mSubPlanets[j]]);
                        if(temp == n) continue;
                        if(4 == i)
                        {
                            temp->setNeighbor(3-j, n);                    
                            n->setNeighbor(NEIGHBOR_UP, temp);
                        }
                        else
                        {
                            temp->setNeighbor(j, n);                    
                            n->setNeighbor(NEIGHBOR_DOWN, temp); 
                        }
                    }
                }
            }
        }
    }
    
    return mChilds[childID];
}

bool PlanetSektor::isObjectInSektor(Vector3Unit positionInSektor)
{    
    Unit radiusSquare = mRadius.convertTo(AE)*6.0;
    radiusSquare *= radiusSquare;
    //Unit distance = Vector3Unit(positionInParentSektor - mSektorPosition).length();
    //return Vector3Unit(positionInParentSektor.convertTo(AE) - mSektorPosition).lengthSq() <= radiusSquare;    
    return positionInSektor.convertTo(AE).lengthSq() <= radiusSquare;    
}