#ifndef __SC_PLANET_SEKTOR__
#define __SC_PLANET_SEKTOR__

class PlanetSektor : public Sektor
{
public:
    PlanetSektor(Vector3Unit position, Unit radius, SektorID id, Sektor* parent);
    virtual ~PlanetSektor();
    
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
    
    __inline__ GenerateNoisePlanet* getNoiseGenerator() {return mNoiseGenerator;}
    
    virtual bool isObjectInSektor(Vector3Unit positionInSektor);
    virtual Sektor* getChild(SektorID childID);
protected:
    GenerateNoisePlanet*        mNoiseGenerator;
        
       
        
private:
    static SektorID mSubPlanets[];
    
};

#endif //__SC_PLANET_SEKTOR__