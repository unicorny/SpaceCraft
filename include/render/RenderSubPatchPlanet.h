#ifndef __SC_RENDER_SUB_PATCH_PLANET__
#define __SC_RENDER_SUB_PATCH_PLANET__

#include "RenderSubPlanet.h"

class RenderSubPatchPlanet : public RenderSubPlanet
{
public:
    RenderSubPatchPlanet(SektorID seed, DRVector3 translate, float cameraDistance, const DRMatrix& rotation, DRString texturePath);

    virtual ~RenderSubPatchPlanet() {};
};

#endif //__SC_RENDER_SUB_PATCH_PLANET__