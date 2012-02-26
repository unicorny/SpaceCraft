#ifndef __SC_SUB_PATCH_PLANET_SEKTOR__
#define __SC_SUB_PATCH_PLANET_SEKTOR__

#include "SubPlanetSektor.h"
#include "RenderSubPatchPlanet.h"

class SubPatchPlanetSektor : public SubPlanetSektor
{
public:
    SubPatchPlanetSektor(Vector3Unit position, Unit radius, SektorID id, Sektor* parent, PlanetSektor* planet,
                    float cameraDistance = 0.0f, int subLevel = 7);
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
    
    //__inline__ void setNeighbor(u8 index, SubPatchPlanetSektor* neighbor) {if(index >= 4) return; mNeighbors[index] = neighbor;}
    
    //virtual bool isObjectInSektor(Vector3Unit positionInSektor);
    
    //virtual Sektor* getChild(SektorID childID);
    
};

#endif //__SC_SUB_PATCH_PLANET_SEKTOR__