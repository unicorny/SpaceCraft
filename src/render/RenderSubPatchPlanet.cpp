#include "RenderSubPatchPlanet.h"

RenderSubPatchPlanet::RenderSubPatchPlanet(SektorID seed, DRVector3 translate, float cameraDistance, const DRMatrix& rotation, DRString texturePath, const PlanetNoiseParameter* planetNoiseParameter)
: RenderSubPlanet(translate, cameraDistance, rotation, texturePath, planetNoiseParameter)
{
    
}