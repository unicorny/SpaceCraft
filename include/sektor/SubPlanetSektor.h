#ifndef __SC_SUB_PLANET_SEKTOR__
#define __SC_SUB_PLANET_SEKTOR__

#include "Sektor.h"

class SubPlanetSektor : public Sektor
{
public:
    SubPlanetSektor(Vector3Unit position, Unit radius, SektorID id, Sektor* parent, int subLevel = 6);
    virtual ~SubPlanetSektor() {}
    
    /*! \brief move sektor
     * 
     * check if child are visible, and move (logical, physical)
     * sektors longer not visible, will be deleted
     * generate all sektors, which are now visible,
     * 
     */
    DRReturn move(float fTime, Camera* cam);
    
    /*! \brief render sektor and childs
     * 
     *  using transformation and render sektor,     *  
     */    
    DRReturn render(float fTime, Camera* cam);
    
    static double Wurzel_3;
    
protected:
    int         mSubLevel;// Level of part of planet, this is a 1/mSubLevel part of the planet
    
    
};

#endif //__SC_SUB_PLANET_SEKTOR__