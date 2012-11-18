#ifndef __SC_PLANET_SEKTOR__
#define __SC_PLANET_SEKTOR__

#include "RenderPlanet.h"
#include "SolarSystemSektor.h"

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
    MIN_HEIGHT_IN_PERCENT = 16,
    TERRAIN_OFFSET = 17,
    MOUNTAINS_TWIST = 18,
    HILLS_TWIST = 19,
    BADLANDS_TWIST = 20
    
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
            float terrainOffset;
            float mountainsTwist;
            float hillsTwist;
            float badlandsTwist;
        };
        float values[21];
    };
};

#define READY_COUNT_BIT_1 1
#define READY_COUNT_BIT_2 2
#define READY_COUNT_BIT_3 4
#define READY_COUNT_BIT_4 8
#define READY_COUNT_BIT_5 16
#define READY_COUNT_BIT_6 32

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
    
    /*! \brief update visibility of sectors for all active cameras
     *  \param cameras list with all active cameras, which currently observe at least one sektor
     */
    virtual DRReturn updateVisibility(const std::list<Camera*>& cameras);
    
    // all orbital objects are inside of the planet sector
    virtual bool isObjectInSektor(SektorObject* sektorObject);
    virtual bool isSectorVisibleFromPosition(Vector3Unit positionInSektor);
    
    // abgeleitet von basis klasse, wird u.a. für die Zuordnung der Kamere verwendet
    virtual Sektor* getChild(SektorID childID);
    __inline__ GLint getShaderProgram() {return mSphericalShaderForSubPlanet->getProgram();}
    __inline__ double getTheta() {return mTheta;}
    __inline__ const PlanetNoiseParameter* getPlanetNoiseParameters() const {return &mPlanetNoiseParameters;}
    
    static SektorID mSubPlanets[];
    
    __inline__ void setReadyCount(u8 readyCountBit) {mReadyCount |= readyCountBit;}
    __inline__ bool isReady() {return 63 == mReadyCount ? true : false;}

protected:
    
    static int      mSubPlanetNeighborIndices[];
    
    ShaderProgram*              mSphericalShaderForSubPlanet;
    double                      mTheta;
    PlanetNoiseParameter        mPlanetNoiseParameters;
    u8                          mReadyCount;
        
private:
    
    
};

#endif //__SC_PLANET_SEKTOR__