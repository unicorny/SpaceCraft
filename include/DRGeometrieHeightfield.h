#ifndef __DR_GEOMETRIE_HEIGHTFIELD__
#define __DR_GEOMETRIE_HEIGHTFIELD__

#include "main.h"

enum GeometrieGridFormat
{
    //! values can be combined
    //! TODO: implement function
    GEO_FULL = 1, //! normal quad
    GEO_LEFT = 2, //! on the left side, the quads have double size, to connect with double sized grid
    GEO_UP = 4,   //! on the upper side, the quads have double size, to connect with double sized grid
    GEO_RIGHT = 8, //! on the right side, the quads have double size, to connect with double sized grid
    GEO_DOWN = 16, //!  on the bottom side, the quads have double size, to connect with double sized grid
    GEO_NONE = 0
};

enum GeometrieVertexFormat
{
    GEO_VERTEX_TRIANGLE_STRIP = 1,
    GEO_VERTEX_QUADS = 2,
    GEO_VERTEX_TRIANGLES = 3
};

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
    
        /*! \brief init GeometrieData for a Heightfield
     * 
     * 
     * \param gridSize square count per row, row count (heightfiel contains gridSize*gridSize squares)
     * \param edgePoints coordinates of the Edge-Points, in counter clockwise order
     *        0 --- 1
     *        |   / |
     *        | /   |
     *        2 --- 3
     * \param format  set geometrie grid format look at GeometrieGridFormat
     * \param vertexFormat currently choose between GEO_VERTEX_TRIANGLE_STRIP and GEO_VERTEX_QUADS
     *         GEO_VERTEX_TRIANGLE_STRIP use the half count of indices like GEO_VERTEX_QUADS and is faster to render,
     *         but it isn't suitable for mountains and cannot be used for a other GeometrieGridFormat as GEO_FULL
     * \return DR_OK if no error occured
     */
    DRReturn init(u32 gridSize, DRVector3 edgePoints[4], GeometrieGridFormat format, GeometrieVertexFormat vertexFormat = GEO_VERTEX_TRIANGLES);    
    
    
protected:
    DRHeightValueStorage*       mHeightValues;
    DRVector3                   mSphericalCenter;
    GLuint			mMemorySize;
};

#endif //__DR_GEOMETRIE_HEIGHTFIELD__