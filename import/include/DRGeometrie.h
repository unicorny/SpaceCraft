/* 
 * File:   DRGeometrie.j
 * Author: dario
 *
 * Created on 25. August 2011, 12:01
 */

#ifndef _DR_ENGINE_GEOMETRIE_h
#define	_DR_ENGINE_GEOMETRIE_h

class ENGINE_API DRGeometrie
{
public:
    DRGeometrie();
    virtual ~DRGeometrie();
    
    /*! \brief malloc memory for geometrie
     * 
     * reserviert Speicher für die Geometrie
     * falls noch Speicher reserviert war, wird dieser freigegeben, und alle Daten gelöscht!
     * \param vertexCount für wie viel Vertices Speicher reserviert werden soll (Vector3, Color und TextureCoords)
     * \param indexCount für wie viele Indices Speicher reserviert werden soll
     * \param textureCount wie viele Textur-Koordinaten-Arrays benötigt werden (Anzahl der Texturen, Multitexturing)
     * \param color bei false wird kein Speicher für das color-array reserviert
     * \return DR_OK wenn alles okay ist, oder DR_ERROR bei Fehler 
     * */
    DRReturn init(u32 vertexCount, u32 indexCount = 0, u32 textureCount = 0, bool color = false);
    
    DRReturn render();
    
    DRVector3* getVertexPointer() {return mVertices;}
    DRColor*   getColorPointer()  {return mColors;}
    GLuint*    getIndexPointer()  {return mIndices;}
    
protected:
    DRVector3*  mVertices;
    GLuint*     mIndices;
    DRColor*    mColors;
    DRVector2** mTextureCoords;
    u32         mVertexCount;
    u32         mIndexCount;
    u32         mNumTextureCoords;
    
    GLenum      mRenderMode;
       
};


#endif	/* _DR_ENGINE_GEOMETRIE_h */

