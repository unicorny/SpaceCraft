#ifndef __SC_SUB_PLANET_SEKTOR__
#define __SC_SUB_PLANET_SEKTOR__

#define NEIGHBOR_LEFT 0
#define NEIGHBOR_UP 1
#define NEIGHBOR_RIGHT 2
#define NEIGHBOR_DOWN 3


class SubPlanetSektor : public Sektor
{
public:
    SubPlanetSektor(Vector3Unit position, Unit radius, SektorID id, Sektor* parent, PlanetSektor* planet, DRVector3 edgePoints[4], int subLevel = 6);
    virtual ~SubPlanetSektor();
    
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
    
    __inline__ void setNeighbor(u8 index, SubPlanetSektor* neighbor) {if(index >= 4) return; mNeighbors[index] = neighbor;}
    
    virtual bool isObjectInSektor(Vector3Unit positionInSektor);
    
    virtual Sektor* getChild(SektorID childID);
    
    //! \param out pointer on DRVector3-Array with at least the size 4
    DRReturn getCurrentSideEdges(DRVector3 out[4]);
    
protected:
    
    void fillEdgePoints();
    void calculateNewEdgePoints(SektorID index, DRVector3 edgePoints[4]);
    
    int                 mSubLevel;// Level of part of planet, this is a 1/mSubLevel part of the planet
    SubPlanetSektor*    mNeighbors[4]; //left, up, right, down
    PlanetSektor*       mPlanet;
    DRVector3           mEdgePoints[4];
    
    // temporäre variablen      
    double              mHorizontCulling;
private:
    
    
};

#endif //__SC_SUB_PLANET_SEKTOR__