#ifndef __SC_SUB_PATCH_PLANET_SEKTOR__
#define __SC_SUB_PATCH_PLANET_SEKTOR__

#include "SubPlanetSektor.h"
#include "RenderSubPatchPlanet.h"

class SubPatchPlanetSektor : public SubPlanetSektor
{
public:
    SubPatchPlanetSektor(Vector3Unit position, Unit radius, SektorID id, Sektor* parent, PlanetSektor* planet,
                    float patchScaling = 1.0f, int subLevel = 7);
    virtual ~SubPatchPlanetSektor();
    
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
    
    /*! \brief update visibility of sectors for all active cameras
     *  \param cameras list with all active cameras, which currently observe at least one sektor
     */
    virtual DRReturn updateVisibility(const std::list<Camera*>& cameras);
    
    //__inline__ void setNeighbor(u8 index, SubPatchPlanetSektor* neighbor) {if(index >= 4) return; mNeighbors[index] = neighbor;}
    
    virtual bool isObjectInSektor(Vector3Unit positionInSektor);
    
    //virtual Sektor* getChild(SektorID childID);
    
protected:
    DRVector3    mVectorToPlanetCenter;        
    
};

#endif //__SC_SUB_PATCH_PLANET_SEKTOR__