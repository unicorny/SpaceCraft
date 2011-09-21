#include "main.h"

StellarBody::StellarBody(Unit radius, Vector3Unit position, int seed, Sektor* parent/* = NULL*/)
: mRadius(radius), mPosition(position), mParent(parent), mSeed(seed)
{    
    mGeometrie.initIcoSphere(3, seed);
}

StellarBody::~StellarBody()
{
    
}

DRReturn StellarBody::render(float fTime)
{
    if(mGeometrie.render())
        LOG_ERROR("Fehler beim rendern der geometrie", DR_ERROR);
    return DR_OK;
}
