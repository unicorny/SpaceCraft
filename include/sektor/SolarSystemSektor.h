#ifndef __SC_SOLAR_SYSTEM_SEKTOR__
#define __SC_SOLAR_SYSTEM_SEKTOR__

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
    
protected:
    virtual bool isPlayerInIt(Camera* cam) {return false;};
    
private:
};

#endif //__SC_SOLAR_SYSTEM_SEKTOR__