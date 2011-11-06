#include "main.h"

double GenerateNoisePlanet::getValue(DRVector3 point)
{
    return baseContinentDef_pe0.GetValue(point.x, point.y, point.z);
}
