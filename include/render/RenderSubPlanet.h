#ifndef __SC_RENDER_SUB_PLANET__
#define __SC_RENDER_SUB_PLANET__

enum RenderGridBoxSide
{
    BOX_FRONT = 0,
    BOX_RIGHT = 1,
    BOX_BACK  = 2,
    BOX_LEFT  = 4,
    BOX_TOP   = 8,
    BOX_BOTTOM= 16
};

class RenderSubPlanet : public RenderSektor
{
public:
    //! \param boxSide 
    RenderSubPlanet(GenerateNoisePlanet* noiseGenerator, RenderGridBoxSide boxSide);
    virtual ~RenderSubPlanet();
    
    virtual DRReturn render(float fTime, Camera* cam);
    
    static int TextureGenerateThread(void* data);        
    
    static DRReturn getBoxSideEdges(RenderGridBoxSide boxSide, DRVector3 edges[4]);
    //! \param out pointer on DRVector3-Array with at least the size 4
    DRReturn getCurrentSideEdges(DRVector3* out);
    
protected:
    PlanetHeightValues*         mHeights;
    GenerateNoisePlanet*        mNoiseGenerator;
    DRGeometrieHeightfield*     mGeometrieGrid;
    DRVector3                   mSides[4];
    static s8                   BoxValues[][3];
    
private:    
    SDL_Thread*				mUpdateThread;
    SDL_sem *				mUpdateThreadSemaphore;
    SDL_mutex*                          mUpdateMutex;
    
};

#endif //__SC_RENDER_SUB_PLANET__