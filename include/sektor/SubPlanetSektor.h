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
    
    /*! \brief render sektor and childs
     * 
     *  using transformation and render sektor,     *  
     */    
    virtual DRReturn render(float fTime, Camera* cam);
    
    __inline__ void setNeighbor(u8 index, SubPlanetSektor* neighbor) {if(index >= 4) return; mNeighbors[index] = neighbor;}
    
    virtual bool isObjectInSektor(Vector3Unit positionInSektor);
    
    virtual Sektor* getChild(SektorID childID);
    
    const DRMatrix& getRotation() {return mRotation;}
    __inline__ DRVector3 getVectorToPlanet() {return mVectorToPlanetCenter;}
    __inline__ DRVector3 getTextureTranslate() {return mTextureTranslate;}
    
    virtual void printTypeInfos(const char* name);
    
protected:
    int                 mSubLevel;// Level of part of planet, this is a 1/mSubLevel part of the planet
    SubPlanetSektor*    mNeighbors[4]; //left, up, right, down
    PlanetSektor*       mPlanet;
    float               mPatchScaling;
    DRMatrix            mRotation;
    //! position relative to planet center
    DRVector3           mVectorToPlanetCenter;
    DRVector3           mTextureTranslate;
    
    static DRVector3    mSubLevelBorder[];
    
    // temporäre variablen      
    //double              mHorizontCulling;
private:
    
    
};

#endif //__SC_SUB_PLANET_SEKTOR__