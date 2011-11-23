#ifndef __SC_RENDER_SUB_PLANET__
#define __SC_RENDER_SUB_PLANET__

enum RenderGridBoxSide
{
    BOX_FRONT = 0,
    BOX_RIGHT = 1,
    BOX_BACK  = 2,
    BOX_LEFT  = 3,
    BOX_TOP   = 4,
    BOX_BOTTOM= 5
};

class RenderSubPlanet : public RenderPlanet
{
public:
    //! \param boxSide 
    RenderSubPlanet(GenerateNoisePlanet* noiseGenerator, RenderGridBoxSide boxSide);
    virtual ~RenderSubPlanet();
    
    virtual DRReturn render(float fTime, Camera* cam);
    
    static int TextureGenerateThread(void* data);        
    
    static DRReturn getBoxSideEdges(RenderGridBoxSide boxSide, DRVector3 edges[4]);
    
protected:
    DRGeometrieHeightfield*     mGeometrieGrid;
    static s8                   BoxValues[][3];
};

#endif //__SC_RENDER_SUB_PLANET__