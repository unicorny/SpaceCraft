#ifndef __SC_RENDER_SUB_PLANET__
#define __SC_RENDER_SUB_PLANET__

#include "RenderPlanet.h"

class RenderSubPlanet : public RenderPlanet
{
public:
    //! \param boxSide 
    RenderSubPlanet(SektorID seed, float theta, float cameraDistance, const DRMatrix& rotation, DRString texturePath);

    virtual ~RenderSubPlanet();
    
    virtual DRReturn render(float fTime, Camera* cam);
    
    
    
protected:
    DRGeometrieHeightfield*     mGeometrieGrid;
    
    //Sphere
        
private:        
    
};

#endif //__SC_RENDER_SUB_PLANET__