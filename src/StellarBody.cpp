#include "main.h"

StellarBody::StellarBody(Unit radius, Vector3Unit position, int seed, Sektor* parent/* = NULL*/)
: mRadius(radius), mPosition(position), mParent(parent), mSeed(seed)
{    
    mGeometrie.initIcoSphere(7, seed);    
    //mGeometrie.initSphere(40);
	mGeometrie.changeGeometrieTo(2, true);
	//mGeometrie.initIcoSphere(1, seed);
    //mGeometrie.initIcoSphere(1, seed);
    //srand(seed);
  /*  mGeometrie.makeSphericalLandscape(500, rand());
    for(int i = 0; i < mGeometrie.getVertexCount(); i++)
    {
        float l = 1.0f - mGeometrie.getVertexPointer()[i] .length();
        l *= 80.0f;
        mGeometrie.getColorPointer()[i] = DRColor((1.0f-l)/2.0f, 1.0f-l, (1.0f-l)/2.0f);
    }
    mGeometrie.copyDataToVertexBuffer();
   //* */
}

StellarBody::~StellarBody()
{
    
}

DRReturn StellarBody::render(float fTime, Vector3Unit absCameraPosition)
{
    glPushMatrix();
    //glTranslatef()
    
    Unit distance1 = Vector3Unit(mPosition - absCameraPosition).length();
    DRVector3 diff = Vector3Unit(mPosition - absCameraPosition).convertTo(KM).getVector3();
    
    
    distance1 = distance1.convertTo(mRadius.getType());
    float distance2 = 200.0f;
    Unit radius1 = mRadius;
    float radius2 = ((radius1 * distance2) / distance1);
    
    DRVector3 pos = (mPosition - absCameraPosition).getVector3().normalize();
    pos *= distance2;
 /*   printf("\r %f %f %f, %.8f, %s  x:%s y:%s z:%s (%f %f %f)", pos.x, pos.y, pos.z, radius2, distance1.print().data(),
                                               absCameraPosition.x.print().data(), absCameraPosition.y.print().data(),
                                               absCameraPosition.z.print().data(), diff.x, diff.y, diff.z);
											   //*/
	if(radius2 > 160.0f) mGeometrie.changeGeometrieTo(7);
	else if(radius2 > 90.0f) mGeometrie.changeGeometrieTo(6);
	else if(radius2 > 60.0f) mGeometrie.changeGeometrieTo(5);
	else if(radius2 > 45.0f) mGeometrie.changeGeometrieTo(4);
	else if(radius2 > 30.0f) mGeometrie.changeGeometrieTo(3);
	else if(radius2 > 10.0f) mGeometrie.changeGeometrieTo(2);
	else if(radius2 > 1.0f) mGeometrie.changeGeometrieTo(1);
	else mGeometrie.changeGeometrieTo(0);

	mGeometrie.update();
/*    if(mCurrentDetail == 0 && radius2 > 1.0f)
    {
        mGeometrie.initIcoSphere(1, mSeed);
        mCurrentDetail = 1;
        printf("update geometrie\n");
    }
    if(mCurrentDetail == 1 && radius2 > 10.0f)
    {
        mGeometrie.initIcoSphere(2, mSeed);
        mCurrentDetail = 2;
        printf("update geometrie2\n");
    }
    if(mCurrentDetail == 2 && radius2 > 30.0f)
    {
        mGeometrie.initIcoSphere(3, mSeed);
        mCurrentDetail = 3;
        printf("update geometrie3\n");
    }
	if(mCurrentDetail == 3 && radius2 > 45.0f)
    {
        mGeometrie.initIcoSphere(4, mSeed);
        mCurrentDetail = 4;
        printf("update geometrie4\n");
    }
	if(mCurrentDetail == 4 && radius2 > 60.0f)
    {
        mGeometrie.initIcoSphere(5, mSeed);
        mCurrentDetail = 5;
        printf("update geometrie5\n");
    }
	if(mCurrentDetail == 5 && radius2 > 90.0f)
    {
        mGeometrie.initIcoSphere(6, mSeed);
        mCurrentDetail = 6;
        printf("update geometrie6\n");
    }
	if(mCurrentDetail == 6 && radius2 > 160.0f)
    {
        mGeometrie.initIcoSphere(7, mSeed);
        mCurrentDetail = 7;
        printf("update geometrie2\n");
    }
    //*/
    glTranslatef(pos.x, pos.y, pos.z);
    glScalef(radius2, radius2, radius2);
    
    if(mGeometrie.render())
        LOG_ERROR("Fehler beim rendern der geometrie", DR_ERROR);
    
    glPopMatrix();
    if(DRGrafikError("[StellarBody::render]")) return DR_ERROR;
    
    return DR_OK;
}
