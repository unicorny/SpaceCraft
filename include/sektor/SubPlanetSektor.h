#ifndef __SC_SUB_PLANET_SEKTOR__
#define __SC_SUB_PLANET_SEKTOR__

#define NEIGHBOR_LEFT 0
#define NEIGHBOR_UP 1
#define NEIGHBOR_RIGHT 2
#define NEIGHBOR_DOWN 3


#define CAMERA_IS_ABOVE_SECTOR 1
#define CAMERA_IS_ABOVE_NEIGHBOR_SECTOR 2
#define CAMERA_IS_ABOVE_NEIGHBOR_EDGE 4
#define CAMERA_ABOVE_SECTOR_SIDE_LEFT 8
#define CAMERA_ABOVE_SECTOR_SIDE_TOP 16
#define CAMERA_ABOVE_SECTOR_SIDE_RIGHT 32
#define CAMERA_ABOVE_SECTOR_SIDE_BOTTOM 64
#define CAMERA_ABOVE_SECTOR_EDGE_TOP_LEFT (CAMERA_ABOVE_SECTOR_SIDE_LEFT | CAMERA_ABOVE_SECTOR_SIDE_TOP)
#define CAMERA_ABOVE_SECTOR_EDGE_TOP_RIGHT (CAMERA_ABOVE_SECTOR_SIDE_RIGHT | CAMERA_ABOVE_SECTOR_SIDE_TOP)
#define CAMERA_ABOVE_SECTOR_EDGE_BOTTOM_LEFT (CAMERA_ABOVE_SECTOR_SIDE_BOTTOM | CAMERA_ABOVE_SECTOR_SIDE_LEFT)
#define CAMERA_ABOVE_SECTOR_EDGE_BOTTOM_RIGHT (CAMERA_ABOVE_SECTOR_SIDE_BOTTOM | CAMERA_ABOVE_SECTOR_SIDE_RIGHT)
#define CAMERA_ABOVE_SECTOR_EDGE (CAMERA_ABOVE_SECTOR_SIDE_LEFT | CAMERA_ABOVE_SECTOR_SIDE_TOP | CAMERA_ABOVE_SECTOR_SIDE_RIGHT | CAMERA_ABOVE_SECTOR_SIDE_BOTTOM)
#define CAMERA_ABOVE_SECTOR_FLAG_FROM_NEIGHBOR 128



#include "PlanetSektor.h"
#include "RenderSubPlanet.h"

class SubPlanetSektor;
typedef DRSimpleResourcePtr<SubPlanetSektor> SubPlanetSektorPtr;

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
    
    __inline__ void setNeighbor(u8 index, SubPlanetSektorPtr* neighbor) {if(index >= 4) return; if(mNeighbors[index]) mNeighbors[index]->removeRef();  mNeighbors[index] = neighbor;}
    __inline__ void setNeighbor(u8 index, Sektor* sektor) {if(index >= 4) return; if(!sektor){ mNeighbors[index] = NULL; return; } if(sektor->isType(SUB_PLANET)) setNeighbor(index, static_cast<SubPlanetSektor*>(sektor)->getMeSave());}
    __inline__ SubPlanetSektorPtr* getNeighbor(u8 index) {if(index >= 4) return NULL; return mNeighbors[index];}
    
    virtual bool isObjectInSektor(SektorObject* sektorObject);
    virtual bool isSectorVisibleFromPosition(Vector3Unit positionInSektor);
    
    virtual Sektor* getChild(SektorID childID);
    
    const DRMatrix& getRotation() const {return mRotation;}
    u8 getRotationsIndex() const {return mCubeSideIndex;}
    __inline__ const DRVector3& getVectorToPlanet() const {return mVectorToPlanetCenter;}
    __inline__ const DRVector3& getTextureTranslate() const {return mTextureTranslate;}
    
    virtual void printTypeInfos(const char* name) const;
    
    __inline__ bool isCameraAbove() {return CAMERA_IS_ABOVE_SECTOR & mCameraAbove;}
    __inline__ bool isCameraAboveNeighbor() {return CAMERA_IS_ABOVE_NEIGHBOR_SECTOR & mCameraAbove;}
    __inline__ bool isCameraAboveNeighborEdge() {return CAMERA_IS_ABOVE_NEIGHBOR_EDGE & mCameraAbove;}
    __inline__ bool isSektorVisibleToRender() {return isCameraAbove() || isCameraAboveNeighbor() || isCameraAboveNeighborEdge();}
    
    void setCameraAboveNeighbor();
    __inline__ void setCameraAboveNeighborEdge() {mCameraAbove |= CAMERA_IS_ABOVE_NEIGHBOR_EDGE;}
    __inline__ void setCameraIsMaybeAboveNeigbhorEdge() {if (mCameraAbove & CAMERA_ABOVE_SECTOR_FLAG_FROM_NEIGHBOR) setCameraAboveNeighborEdge(); else mCameraAbove |= CAMERA_ABOVE_SECTOR_FLAG_FROM_NEIGHBOR;}
    void resetCameraAbove(int deep = 0);

    virtual DRString getSektorPathName();

    __inline__ DRSimpleResourcePtr<SubPlanetSektor>* getMeSave() {mThis->addRef(); return mThis;}
        
protected:
    void informNeighborAboutCameraAbove();
    float calculateDistanceToNeighbor(Sektor* neighbor);

    SubPlanetSektorPtr* mThis;
    int                 mSubLevel;// Level of part of planet, this is a 1/mSubLevel part of the planet
    SubPlanetSektorPtr*    mNeighbors[4]; //left, up, right, down
    PlanetSektor*       mPlanet;
    float               mPatchScaling;
    DRMatrix            mRotation;
    u8                  mCubeSideIndex;
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