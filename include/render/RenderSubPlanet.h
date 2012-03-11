#ifndef __SC_RENDER_SUB_PLANET__
#define __SC_RENDER_SUB_PLANET__

#include "RenderPlanet.h"

class RenderSubPlanet : public RenderPlanet
{
public:
    //! \param boxSide 
    RenderSubPlanet(SektorID seed, DRVector3 translate, float patchScaling, const DRMatrix& rotation, DRString texturePath, const PlanetNoiseParameter* planetNoiseParameter);

    virtual ~RenderSubPlanet();
    
    virtual DRReturn render(float fTime, Camera* cam);
    
    
    
protected:
    DRGeometrieHeightfield*     mGeometrieGrid;
    
    //Sphere
        
private:        
    
};

#endif //__SC_RENDER_SUB_PLANET__