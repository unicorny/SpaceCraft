#ifndef __SC_PLANET_SEKTOR__
#define __SC_PLANET_SEKTOR__

#include "RenderPlanet.h"

enum PlanetNoiseParameterNames
{
    CONTINENT_FREQUENCY = 0,
    CONTINENT_LACUNARITY = 1,
    MOUNTAIN_LACUNARITY = 2,
    HILLS_LACUNARITY = 3,
    PLAINS_LACUNARITY = 4,
    BADLANDS_LACUNARITY = 5,
    MOUNTAIN_GLACIATION = 6,
    SEA_LEVEL = 7,
    SHELF_LEVEL = 8,
    MOUNTAINS_AMOUNT = 9,
    HILLS_AMOUNT = 10,
    BADLANDS_AMOUNT = 11,
    RIVER_DEPTH = 12,
    CONTINENT_HEIGHT_SCALE = 13,
    SEA_LEVEL_IN_METRES = 14,
    MAX_HEIGHT_IN_PERCENT = 15,
    MIN_HEIGHT_IN_PERCENT = 16
};

struct PlanetNoiseParameter
{
    PlanetNoiseParameter()
    {memset(values, 0, sizeof(float)*15); }
    void print(bool toLog = false);
    const char* getFieldName(PlanetNoiseParameterNames feldName);
    union
    {
        struct 
        {
            float continentFrequenzy;
            float continentLacunarity;
            float mountainLacunarity;
            float hillsLacunarity;
            float plainsLacunarity;
            float badlandsLacunarity;
            float mountainGlaciation;
            float seaLevel;
            float shelfLevel;
            float mountainAmount;
            float hillsAmount;
            float badlandsAmount;
            float riverDeapth;
            float continentHeightScale;
            float seaLevelInMetres;
            float maxHeightInPercent;
            float minHeightInPercent;
        };
        float values[17];
    };
};

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
    
    virtual bool isObjectInSektor(Vector3Unit positionInSektor);
    
    // abgeleitet von basis klasse, wird u.a. für die Zuordnung der Kamere verwendet
    virtual SektorPtr getChild(SektorID childID);
    __inline__ GLint getShaderProgram() {return mSphericalShaderForSubPlanet->getProgram();}
    __inline__ double getTheta() {return mTheta;}
    __inline__ const PlanetNoiseParameter* getPlanetNoiseParameters() const {return &mPlanetNoiseParameters;}
    
protected:
    ShaderProgram*              mSphericalShaderForSubPlanet;
    double                      mTheta;
    PlanetNoiseParameter        mPlanetNoiseParameters;
        
private:
    static SektorID mSubPlanets[];
    
};

#endif //__SC_PLANET_SEKTOR__