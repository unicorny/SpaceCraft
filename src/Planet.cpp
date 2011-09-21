#include "main.h"

Planet::Planet(Unit radius, Vector3Unit position, int seed, Sektor* parent/* = NULL*/)
: StellarBody(radius, position, seed, parent)
{
    DRLog.writeToLog("Generate new Planet with Radius: %s", radius.print().data());
}

Planet::~Planet()
{
    
}
