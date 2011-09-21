#include "main.h"

StellarBody::StellarBody(Unit radius, Unit position[3], int seed, Sektor* parent/* = NULL*/)
: mRadius(radius), mParent(parent), mSeed(seed)
{
    memcpy(mPosition, position, sizeof(Unit)*3);
    mGeometrie.initIcoSphere(3, seed);
}

StellarBody::~StellarBody()
{
    
}

DRReturn StellarBody::render(float fTime)
{
    return DR_OK;
}
