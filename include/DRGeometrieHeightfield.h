#ifndef __DR_GEOMETRIE_HEIGHTFIELD__
#define __DR_GEOMETRIE_HEIGHTFIELD__

class DRGeometrieHeightfield : public DRGeometrie
{
public:
    DRGeometrieHeightfield(DRVector3 sphericalCenter = DRVector3(0.0f));
    virtual ~DRGeometrieHeightfield();
    
    /*! \brief init GeometrieData for a Heightfield
     * 
     * 
     * \param edgePoints coordinates of the Edge-Points, in counter clockwise order
     *        0 --- 1
     *        |   / |
     *        | /   |
     *        2 --- 3
     * \param gridSize square count per row, row count (heightfiel contains gridSize*gridSize squares)
     * \param heightValues object wich return the heighValues
     * \param normals if set to true, memory for normals are calculate and normals
     * \param color if set to true, space for color will be allocated, and colors will be set throw heightValues
     * \param textureCount how many texture coordinate sets will be allocated
     * \return DR_OK if no error occured
     */
    DRReturn initHeightfield(DRVector3 edgePoints[4], u32 gridSize, DRHeightValueStorage* heightValues, bool normals = true, bool color = true, u8 textureCount = 0);
    
    
protected:
    DRHeightValueStorage*       mHeightValues;
    DRVector3                   mSphericalCenter;
};

#endif //__DR_GEOMETRIE_HEIGHTFIELD__