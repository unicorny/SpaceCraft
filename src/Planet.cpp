#include "main.h"

Planet::Planet(Unit radius, Unit position[3], int seed, Sektor* parent/* = NULL*/)
: StellarBody(radius, position, seed, parent)
{
    DRLog.writeToLog("Generate new Planet with Radius: %s", radius.print().data());
}

Planet::~Planet()
{
    
}
