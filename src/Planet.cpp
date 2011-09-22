#include "main.h"

Planet::Planet(Unit radius, Vector3Unit position, int seed, Sektor* parent/* = NULL*/)
: StellarBody(radius, position, seed, parent)
{
    DRLog.writeToLog("[Planet::Planet] Generate new Planet with Radius: %s, on Position:", radius.print().data());
    position.print("[Planet::Planet] position");
    
}

Planet::~Planet()
{
    
}
