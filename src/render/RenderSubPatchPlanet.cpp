#include "RenderSubPatchPlanet.h"

RenderSubPatchPlanet::RenderSubPatchPlanet(SektorID seed, DRVector3 translate, float cameraDistance, const DRMatrix& rotation, DRString texturePath, const PlanetNoiseParameter* planetNoiseParameter)
: RenderSubPlanet(seed, translate, cameraDistance, rotation, texturePath, planetNoiseParameter, false)
{
    
}