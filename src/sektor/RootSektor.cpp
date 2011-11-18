#include "main.h"

RootSektor::RootSektor()
: Sektor(Vector3Unit(0.0), Unit(0.0, M), 0, NULL)
{
    mType = SEKTOR_ROOT;
}

RootSektor::~RootSektor()
{
    
}