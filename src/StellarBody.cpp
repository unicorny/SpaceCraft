#include "main.h"

StellarBody::StellarBody(Unit radius, Vector3Unit position, int seed, Sektor* parent/* = NULL*/)
: mRadius(radius), mPosition(position), mParent(parent), mSeed(seed)
{   
    GenerateNoisePlanet* p = GlobalRenderer::Instance().getGenerateNoisePlanet();
    p->setupGenerator(seed);
    mHeightValues.setNoisePlanet(p);
    mGeometrie.setHeightValueStorage(&mHeightValues);
    
    double seaLevelInMeters = p->getSeaLevelInMetres();
    
    mHeightValues.ClearGradientPoints ();
    mHeightValues.AddGradientPoint (-2.0 + seaLevelInMeters, noise::utils::Color (  0,   0,   0, 255));
    mHeightValues.AddGradientPoint (    -0.03125 + seaLevelInMeters, noise::utils::Color (  6,  58, 127, 255));
    mHeightValues.AddGradientPoint (    -0.0001220703 + seaLevelInMeters, noise::utils::Color ( 14, 112, 192, 255));
    mHeightValues.AddGradientPoint (     0.0 + seaLevelInMeters, noise::utils::Color ( 70, 120,  60, 255));
    mHeightValues.AddGradientPoint (  0.125 + seaLevelInMeters, noise::utils::Color (110, 140,  75, 255));
    mHeightValues.AddGradientPoint (  0.25 + seaLevelInMeters, noise::utils::Color (160, 140, 111, 255));
    mHeightValues.AddGradientPoint (  0.375 + seaLevelInMeters, noise::utils::Color (184, 163, 141, 255));
    mHeightValues.AddGradientPoint (  0.5 + seaLevelInMeters, noise::utils::Color (255, 255, 255, 255));
    mHeightValues.AddGradientPoint (  0.75 + seaLevelInMeters, noise::utils::Color (128, 255, 255, 255));
    mHeightValues.AddGradientPoint ( 2.0 + seaLevelInMeters, noise::utils::Color (  0,   0, 255, 255));
    
    mGeometrie.initIcoSphere(8, seed);    
    //mGeometrie.initSphere(40);
    mGeometrie.changeGeometrieTo(1, true);
    
    
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
    DRVector3 relCameraPos = -pos*distance1/mRadius;
    pos *= distance2;
 /*   printf("\r %f %f %f, %.8f, %s  x:%s y:%s z:%s (%f %f %f)", pos.x, pos.y, pos.z, radius2, distance1.print().data(),
                                               absCameraPosition.x.print().data(), absCameraPosition.y.print().data(),
                                               absCameraPosition.z.print().data(), diff.x, diff.y, diff.z);
    											   //*/
    if(radius2 > 140.0f) mGeometrie.changeGeometrieTo(9, false, relCameraPos);
    else if(radius2 > 120.0f) mGeometrie.changeGeometrieTo(8, false, relCameraPos);
    else if(radius2 > 90.0f) mGeometrie.changeGeometrieTo(7, false, relCameraPos);
    else if(radius2 > 70.0f) mGeometrie.changeGeometrieTo(6, false, relCameraPos);
    else if(radius2 > 30.0f) mGeometrie.changeGeometrieTo(5, false, relCameraPos);
    else if(radius2 > 25.0f) mGeometrie.changeGeometrieTo(4, false, relCameraPos);
    else if(radius2 > 15.0f) mGeometrie.changeGeometrieTo(3, false);
    else if(radius2 > 5.0f) mGeometrie.changeGeometrieTo(2, false);
    else if(radius2 > 1.0f) mGeometrie.changeGeometrieTo(1, false);
    else mGeometrie.changeGeometrieTo(0, false);

    uint quadricDetails = 32;
    if(radius2 > 160.0f) quadricDetails = 256;
    else if(radius2 > 90.0f) quadricDetails = 128;
    else if(radius2 > 20.0f) quadricDetails = 64;
    else if(radius2 > 1.0f)  quadricDetails = 32;
    else quadricDetails = 8;
    
    mGeometrie.update();

    glTranslatef(pos.x, pos.y, pos.z);
    glScalef(radius2, radius2, radius2);
    //GlobalRenderer::getSingleton().getPlanetShaderPtr()->bind();
    if(mGeometrie.render())
       LOG_ERROR("Fehler beim rendern der geometrie", DR_ERROR);
    //gluSphere(GlobalRenderer::Instance().getQuadric(), 1.0f, quadricDetails*2, quadricDetails);
    GlobalRenderer::getSingleton().getPlanetShaderPtr()->unbind();
    glPopMatrix();
    if(DRGrafikError("[StellarBody::render]")) return DR_ERROR;
    
    return DR_OK;
}
