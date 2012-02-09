#ifndef __SC_RENDER_SUB_PLANET__
#define __SC_RENDER_SUB_PLANET__

enum RenderGridBoxSide
{
    BOX_FRONT = 0,
    BOX_RIGHT = 1,
    BOX_BACK  = 2,
    BOX_LEFT  = 4,
    BOX_TOP   = 8,
    BOX_BOTTOM= 16,
    BOX_CENTER = 32
};

class RenderSubPlanet : public RenderSektor
{
public:
    //! \param boxSide 
    RenderSubPlanet(GenerateNoisePlanet* noiseGenerator, PlanetHeightValues* heights, DRVector3 boxSideEdges[4], DRVector3 sphericalCenter);

    virtual ~RenderSubPlanet();
    
    virtual DRReturn render(float fTime, Camera* cam);
    
    static int TextureGenerateThread(void* data);        
    
    
protected:
    PlanetHeightValues*         mHeights;
    GenerateNoisePlanet*        mNoiseGenerator;
    DRGeometrieHeightfield*     mGeometrieGrid;
    
    //Sphere
    float                       mSphereH;
    float                       mTheta;
    DRVector3                   mSphercialCenter;
    
    
private:    
    SDL_Thread*				mUpdateThread;
    SDL_sem *				mUpdateThreadSemaphore;
    SDL_mutex*                          mUpdateMutex;
    
};

#endif //__SC_RENDER_SUB_PLANET__