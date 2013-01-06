#ifndef __SC_RENDER_SUB_PLANET__
#define __SC_RENDER_SUB_PLANET__

#include "RenderPlanet.h"

class RenderSubPlanet : public RenderPlanet
{
public:
    //! \param boxSide 
    RenderSubPlanet(DRVector3 translate, float patchScaling,
                    const DRMatrix& rotation, DRString texturePath, 
                    const PlanetNoiseParameter* planetNoiseParameter);
    RenderSubPlanet(DRTexturePtr parentTexture);

    virtual ~RenderSubPlanet();
    
    virtual DRReturn render(float fTime, Camera* cam);
       
protected:
    DRGeometrieHeightfield*     mGeometrieGrid;
    DRTexturePtr                mParentTexture;
    //Sphere
        
private:        
    
};

#endif //__SC_RENDER_SUB_PLANET__