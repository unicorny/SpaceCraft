#ifndef __SC_SOLAR_SYSTEM_SEKTOR__
#define __SC_SOLAR_SYSTEM_SEKTOR__

#include "PlanetSektor.h"

class SolarSystemSektor : public Sektor
{
public:
    SolarSystemSektor(Vector3Unit position, Unit radius, SektorID id, Sektor* parent);
    virtual ~SolarSystemSektor();
    
    /*! \brief render sektor and childs
     * 
     *  using transformation and render subsektors
     */    
    virtual DRReturn render(float fTime, Camera* cam);
    /*! \brief move sektor
     * 
     * check if child are visible, and move (logical, physical)
     * sektors longer not visible, will be deleted
     * generate all sektors, which are now visible
     */
    virtual DRReturn move(float fTime, Camera* cam);
    
    /*! \brief update visibility of sectors for all active cameras
     *  \param cameras list with all active cameras, which currently observe at least one sektor
     */
    virtual DRReturn updateVisibility(const std::list<Camera*>& cameras);
    
protected:
    
private:
};

#endif //__SC_SOLAR_SYSTEM_SEKTOR__