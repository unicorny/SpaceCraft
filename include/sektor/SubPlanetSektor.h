#ifndef __SC_SUB_PLANET_SEKTOR__
#define __SC_SUB_PLANET_SEKTOR__

#define NEIGHBOR_LEFT 0
#define NEIGHBOR_UP 1
#define NEIGHBOR_RIGHT 2
#define NEIGHBOR_DOWN 3

#include "PlanetSektor.h"
#include "RenderSubPlanet.h"

class SubPlanetSektor : public Sektor
{
public:
    SubPlanetSektor(Unit radius, SektorID id, Sektor* parent, PlanetSektor* planet,
                    float patchScaling = 1.0f, int subLevel = 6);
    virtual ~SubPlanetSektor();
    
    /*! \brief move sektor
     * 
     * check if child are visible, and move (logical, physical)
     * sektors longer not visible, will be deleted
     * generate all sektors, which are now visible,
     * 
     */
    virtual DRReturn move(float fTime, Camera* cam);
    
    /*! \brief update visibility of sectors for all active cameras
     *  \param cameras list with all active cameras, which currently observe at least one sektor
     */
    virtual DRReturn updateVisibility(const std::list<Camera*>& cameras);
    
    /*! \brief render sektor and childs
     * 
     *  using transformation and render sektor,     *  
     */    
    virtual DRReturn render(float fTime, Camera* cam);
    
    __inline__ void setNeighbor(u8 index, SubPlanetSektor* neighbor) {if(index >= 4) return; mNeighbors[index] = neighbor;}
    __inline__ void setNeighbor(u8 index, Sektor* sektor) {if(index >= 4) return; if(!sektor){ mNeighbors[index] = NULL; return; } if(sektor->isType(SUB_PLANET)) setNeighbor(index, static_cast<SubPlanetSektor*>(sektor));}
    __inline__ SubPlanetSektor* getNeighbor(u8 index) {if(index >= 4) return NULL; return mNeighbors[index];}
    
    virtual bool isObjectInSektor(SektorObject* sektorObject);
    virtual bool isSectorVisibleFromPosition(Vector3Unit positionInSektor);
    
    virtual Sektor* getChild(SektorID childID);
    
    const DRMatrix& getRotation() const {return mRotation;}
    u8 getRotationsIndex() const {return mRotationsIndex;}
    __inline__ const DRVector3& getVectorToPlanet() const {return mVectorToPlanetCenter;}
    __inline__ const DRVector3& getTextureTranslate() const {return mTextureTranslate;}
    
    virtual void printTypeInfos(const char* name) const;
    
    __inline__ bool isCameraAbove() {return 1 == mCameraAbove;}
    __inline__ bool isCameraAboveNeighbor(int deep = 0) {checkNeighbor(deep+1); return 2 == mCameraAbove;}
        
protected:
    void checkNeighbor(int deep = 0);
    float calculateDistanceToNeighbor(Sektor* neighbor);

    int                 mSubLevel;// Level of part of planet, this is a 1/mSubLevel part of the planet
    SubPlanetSektor*    mNeighbors[4]; //left, up, right, down
    PlanetSektor*       mPlanet;
    float               mPatchScaling;
    DRMatrix            mRotation;
    u8                  mRotationsIndex;
    u8                  mCameraAbove;
    
   
    //! position relative to planet center
    DRVector3           mVectorToPlanetCenter;
    DRVector3           mTextureTranslate;
    
    static DRVector3    mSubLevelBorder[];
    static DRMatrix     mRotations[];
    static int          mNeighborIndices[];
    static int          mNeighborSpecialIndices[];
    
    // temporäre variablen      
    //double              mHorizontCulling;
private:
    
    
};

#endif //__SC_SUB_PLANET_SEKTOR__