#ifndef __DR_GEOMETRIE_MANAGER__
#define __DR_GEOMETRIE_MANAGER__

#include "main.h"
#include "DRGeometrieHeightfield.h"

class DRGeometrieManager
{
public:
    ~DRGeometrieManager() {if(isInitialized()) exit();};

    // Singleton-Daten
    static DRGeometrieManager&		Instance();
    inline static DRGeometrieManager& getSingleton() {return Instance();};
    inline static DRGeometrieManager* getSingletonPtr() {return &Instance();};
    static bool	isInitialized()	{return Instance().mInitalized;};
    
    DRReturn init();
    DRGeometrieHeightfield* getGrid(GLuint gridSize, GeometrieGridFormat gridFormat, GeometrieVertexFormat vertexFormat = GEO_VERTEX_TRIANGLES);
    void freeGrid(GLuint gridSize, GeometrieGridFormat gridFormat, GeometrieVertexFormat vertexFormat = GEO_VERTEX_TRIANGLES) {};
    
    void exit();
    
private:
    DRGeometrieManager();
    DHASH makeGeometrieHash(GLuint gridSize, GeometrieGridFormat format, GeometrieVertexFormat vertexFormat);    
    
    struct GeometrieEntry
    {
        GeometrieEntry(): geometrie(NULL), referenzCounter(0){}
        DRGeometrie* geometrie;
        int referenzCounter;
    };
    DRHashList  mGeometrieEntrys;
    
    bool mInitalized;
};


#endif //__DR_GEOMETRIE_MANAGER__